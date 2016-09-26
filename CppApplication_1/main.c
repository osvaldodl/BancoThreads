/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: dino_
 *
 * Created on 25 de Setembro de 2016, 21:22
 */

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<semaphore.h>

int _lugares = 5;
int _clientes = 0;

void cliente(void *);
void banco(void *);

sem_t fila_espera;
sem_t cliente;
sem_t mutex;

int 
main(){
	pthread_t cliente_t, banco_t;

	sem_init(&mutex, 0, 1);
	sem_init(&cliente, 0, 1);
	sem_init(&fila_espera, 0, 0);

	pthread_create(&banco_t, NULL, (void *) banco, NULL);
	pthread_create(&cliente_t, NULL, (void *) cliente, NULL);

	pthread_join(banco_t, NULL);
	pthread_join(cliente_t, NULL);

	return EXIT_SUCCESS;
}

void cliente (void *n){
	while(1){
		sem_wait(&mutex); //down
		if(_clientes < _lugares){//tem espaco...
			_clientes++;
			sem_post(&mutex); //up mutex
			sem_post(&fila_espera); //up fila espera
			fprintf(stderr,"novo cliente esperando...\n");
		}else{
			fprintf(stderr,"banco lotado...\n");
			sem_post(&mutex);//nao tem espaco, libera o mutex e segue
		}
		sleep(1);
	}
}

void banco(void *n){
	while(1){
		sem_wait(&fila_espera); //down fila
		sem_wait(&mutex); //down mutex
		_clientes--;
		sem_post(&mutex);	//up mutex
		fprintf(stderr,"barbeiro trabalhando...\n");
		sleep(5);
//		sem_post(&cadeira_barbeiro); //up cadeira
	}
}

