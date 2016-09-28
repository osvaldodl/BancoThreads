#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<time.h>
#include<semaphore.h>

#define C1 3342
#define C2 2145
#define	C3 5543
#define VMAX 100
#define SHMSZ 3




void caixa(void *);
void gerenciador_vendas(void *);
void inicializa_vendas();
	
int banco = 0;
int espera[3];
int vendas[3][VMAX];

int *shm;
sem_t fila_espera;
sem_t mutex;

int main(){
 

	int shmid;
	key_t key;
	long pid_filho;
	int i;
	espera[0]= 0;
	srand(time(NULL));
	
	key = 567194;

	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(1);
	}

	if ((shm = shmat(shmid, NULL, 0)) == (int *) -1) {
		perror("shmat");
		exit(1);
	}
	
	
	pid_filho = fork();		
	inicializa_vendas(&vendas);
	sem_init(&mutex, 0, 1);
	sem_init(&fila_espera, 0, 0);




	if(pid_filho == 0){
		//int vendas[3][VMAX];
		
		
		while(1){
			while(shm[0] == 0);	
			for(i = 0; i < 3; i++)
				if(shm[0] == vendas [i][0] && vendas[i][2] < VMAX ){
					vendas[i][vendas[i][2]] = shm[1];	// ultima posicao escrita do vetor
					vendas[i][1] += shm[1];
					banco += shm[1];
					vendas[i][2]++;
					fprintf(stdout, "Filho produziu %d: %d, posicao %d\n ", i, shm[1], vendas[i][2]);
					shm[0]=0;
				}
		
		}
		
		
		
	}else if (pid_filho != 0){
		
		
		pthread_t caixa1_t, caixa2_t, caixa3_t, gerenciador;

		pthread_create(&gerenciador, NULL, (void *) gerenciador_vendas, NULL);
		pthread_create(&caixa1_t, NULL, (void *) caixa, (void *) C1);
		pthread_create(&caixa2_t, NULL, (void *) caixa, (void *) C2);
		pthread_create(&caixa3_t, NULL, (void *) caixa, (void *) C3);
		pthread_join(gerenciador, NULL);
		pthread_join(caixa1_t, NULL);
		pthread_join(caixa2_t, NULL);
		pthread_join(caixa3_t, NULL);
		
		
		
		
		
	}

  return EXIT_SUCCESS;
}






void caixa (void *n){
	int n_caixa = (int ) n;
	//printf("recebi um numero %d \n", n_caixa);
	while(1){
	    	int valor = rand() % 100;
		fprintf(stderr,"Caixa de %d produziu R$ %d\n",n_caixa, valor);
	     	while(valor > 0){
			while(espera[0] != 0);
			if(sem_wait(&mutex) == 0){
				espera[0]= n_caixa;
				espera[1] = valor;
				valor = 0;
				sem_post(&mutex);
			}else {
				printf("Caixa %d esperara com valor: %d ", n_caixa, valor);
			}
			sleep(rand()%5);
		}
	}
}


void gerenciador_vendas(void *n){
		while(1){
		while(espera[0] == 0);
		sem_wait(&mutex);
		 //down mutex
		printf("caixa %d passado prara p1 com valor %d\n", espera[0], espera[1]);
		shm[0]= espera[0];
		sleep(1);
		shm[1]= espera[1];
		espera[0]=0;	
		sem_post(&mutex);		
		
		}
	
}


void inicializa_vendas(){
	
	
	
	vendas[0][0] = C1; //primeira posicao do vetor armazena o ID do caixa
	vendas[0][1] = 0; // segunda armazena o total que o caixa ja produziu em valor
	vendas[0][2] = 3; // terceira armazena a ultima posicao escrita no vetor, inicializada com 3 pois sera a primeira posicao onde vai conter os valores gerados
	
	vendas[1][0] = C2;
	vendas[1][1] = 0;
	vendas[1][2] = 3;

	vendas[2][0] = C3;
	vendas[2][1] = 0;
	vendas[2][2] = 3;	
}
