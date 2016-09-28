#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<semaphore.h>


#define C1 3342
#define C2 2145
#define	C3 5543



int vendas[3][100], cache[3][10];

void rapido(void *);
void comum(void *);

int banco = 0;

int main(){
 

	int shmid;
	key_t key;
	int *shm;
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
		
		
		
		
		
		
	}else if (pid_filho != 0){
		
		pthread_t rapido_t,comum_t;
		pthread_create(&rapido_t, NULL, (void *) rapido, NULL);
		pthread_create(&comum_t, NULL, (void *) comum, NULL);
		pthread_join(rapido_t, NULL);
		pthread_join(comum_t, NULL);
		
		
		
		
		
	}
	
 
 
 
 
 

 
	
	
	
  return EXIT_SUCCESS;
}

void rapido (void *n){
	while(1){
    int valor = rand() % 100;
    if(valor > 0){
      banco = banco + valor;
      fprintf(stderr,"Caixa rapido inserindo valor de R$ %d\n",valor);
    }
		sleep(2);
	}
}

void comum(void *n){
  while(1){
    int valor = rand() % 200;
    if(valor > 0){
      banco = banco + valor;
      fprintf(stderr,"Caixa comum inserindo valor de R$ %d\n",valor);
    }
		sleep(4);
	}
}

void inicializa_vendas(){
	vendas[1][0] = C1;
	vendas[2][0] = C2;
	vendas[3][0] = C3;	
}