#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<semaphore.h>

void rapido(void *);
void comum(void *);

int banco = 0;

int main(){
  pthread_t rapido_t,comum_t;
  pthread_create(&rapido_t, NULL, (void *) rapido, NULL);
	pthread_create(&comum_t, NULL, (void *) comum, NULL);
	pthread_join(rapido_t, NULL);
	pthread_join(comum_t, NULL);
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
