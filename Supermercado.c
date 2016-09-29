#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<time.h>
#include<semaphore.h>

#define C1 3342 //id unico de cada caixa
#define C2 2145
#define	C3 5543
#define C4 8747
#define C5 4412
#define QTDC 5  //quantidade de caixas

#define VMAX 100
#define SHMSZ 3




void caixa(void *);
void gerenciador_vendas(void *);
void inicializa_vendas(int vendas[QTDC][VMAX]);

int espera[3];


int *shm;
sem_t mutex;

int main(){
 

	int shmid;
	key_t key;
	long pid_filho;
	
	
	
	key = 856941;

	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(1);
	}

	if ((shm = shmat(shmid, NULL, 0)) == (int *) -1) {
		perror("shmat");
		exit(1);
	}
	
	srand(time(NULL));
	pid_filho = fork();		
	sem_init(&mutex, 0, 1);
	shm[0] = 0;
	espera[0]= 0;


	if(pid_filho == 0){

		int vendas[3][VMAX],i , banco =0;// banco armazena o valor total de todos os caixas
		inicializa_vendas(vendas);
		
		while(1){
			while(shm[0] == 0);	
			for(i = 0; i < QTDC; i++)
				if(shm[1] == vendas [i][0] && vendas[i][2] < VMAX ){
					vendas[i][vendas[i][2]] = shm[2];	// ultima posicao escrita do vetor
					vendas[i][1] += shm[2];
					banco += shm[2];
					vendas[i][2]++;
					fprintf(stderr,"Gravando do caixa %d valor: %d, na posicao %d\n\n ", vendas[i][0], shm[2], vendas[i][2]);
					shm[0]= 0;
				}
		
		}
		
		
		
	}else if (pid_filho != 0){
		
		
		pthread_t caixa1_t, caixa2_t, caixa3_t, caixa4_t, caixa5_t,gerenciador;

		pthread_create(&gerenciador, NULL, (void *) gerenciador_vendas, NULL);
		pthread_create(&caixa1_t, NULL, (void *) caixa, (void *) C1);
		pthread_create(&caixa2_t, NULL, (void *) caixa, (void *) C2);
		pthread_create(&caixa3_t, NULL, (void *) caixa, (void *) C3);
		pthread_create(&caixa4_t, NULL, (void *) caixa, (void *) C4);
		pthread_create(&caixa5_t, NULL, (void *) caixa, (void *) C5);

		pthread_join(gerenciador, NULL);
		pthread_join(caixa1_t, NULL);
		pthread_join(caixa2_t, NULL);
		pthread_join(caixa3_t, NULL);
		pthread_join(caixa4_t, NULL);
		pthread_join(caixa5_t, NULL);
		
		
		
	}

  return EXIT_SUCCESS;
}






void caixa (void *n){
	int n_caixa = (int ) n;
	while(1){
	    	int valor = rand() % 100;
		fprintf(stderr,"Caixa %d produziu: R$ %d\n\n",n_caixa, valor);
	     	while(valor > 0){
			while(espera[0] != 0);	// espera bloqueante enquanto houver valor na fila 
			if(sem_wait(&mutex) == 0){
				espera[0]= n_caixa;	//escreve na fila de espera 
				espera[1]= valor;
				valor= 0;
				sem_post(&mutex);
			}


			sleep(rand()%15);
		}
	}
}


void gerenciador_vendas(void *n){
	while(1){
		while(shm[0]== 1 || espera[0] == 0); //espera enquanto nao houver valor na fila ou a memoria compartilhada ainda conter valor nao lido
		sem_wait(&mutex);
		fprintf(stderr,"Caixa %d passado para gravacao com valor %d\n\n", espera[0], espera[1]);
		shm[1]= espera[0];		//passa o valor para o processamento 
		shm[2]= espera[1];
		shm[0]= 1;
		espera[0]= 0;	
		sem_post(&mutex);		
		
	}
	
}


void inicializa_vendas(int vendas[QTDC][VMAX]){
	
	
	vendas[0][0] = C1; //primeira posicao do vetor armazena o ID do caixa
	vendas[0][1] = 0; // segunda armazena o total que o caixa ja produziu em valor
	vendas[0][2] = 3; // terceira armazena a ultima posicao escrita no vetor, inicializada com 3 pois sera a primeira posicao onde vai conter os valores gerados
	
	vendas[1][0] = C2;
	vendas[1][1] = 0;
	vendas[1][2] = 3;

	vendas[2][0] = C3;
	vendas[2][1] = 0;
	vendas[2][2] = 3;

	vendas[3][0] = C4;
	vendas[3][1] = 0;
	vendas[3][2] = 3;

	vendas[4][0] = C5;
	vendas[4][1] = 0;
	vendas[4][2] = 3;	
}
