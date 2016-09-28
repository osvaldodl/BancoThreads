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


int vendas[3][VMAX], cache[3][10];

void rapido(void *);
void comum(void *);
void inicializa_vendas();
	
int banco = 0;
int *shm;

int main(){
 

	int shmid;
	key_t key;
	long pid_filho;
	int i;
	
	
	key = 567194;

	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(1);
	}

	if ((shm = shmat(shmid, NULL, 0)) == (int *) -1) {
		perror("shmat");
		exit(1);
	}
	
	inicializa_vendas();
	pid_filho = fork();		
	
	if(pid_filho == 0){
		while(1){
			while(shm[0] == 0);	
			for(i = 0; i < 3; i++)
				if(shm[0] == vendas [i][0] && vendas[i][2] < VMAX){
					vendas[i][vendas[i][2]] = shm[1];	// ultima posicao escrita do vetor
					vendas[i][1] += shm[1];
					banco += shm[1];
					vendas[i][2]++;
					fprintf(stdout, "Filho produziu %d: %d, posicao %d\n ", i, shm[1], vendas[i][2]);
					shm[0]=0;
				}
		
		}
		
		
		
	}else if (pid_filho != 0){
		
		pthread_t caixa1_t, caixa2_t, caixa3_t;


		pthread_create(&caixa1_t, NULL, (void *) caixa, NULL);
		pthread_create(&caixa2_t, NULL, (void *) caixa, NULL);
		pthread_create(&caixa3_t, NULL, (void *) caixa, NULL);
		pthread_join(caixa1_t, NULL);
		pthread_join(caixa2_t, NULL);
		pthread_join(caixa3_t, NULL);
		
		
		
		
		
	}
	
 
 
 
 
 

 
	
	
	
  return EXIT_SUCCESS;
}

void caixa (void *n){
	while(1){
    int valor = rand() % 100;
    if(valor > 0){
    	shm[0]= C1;
	shm[1] = valor;
      fprintf(stderr,"Caixa rapido inserindo valor de R$ %d\n",valor);
    }
		sleep(2);
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
