#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "sharedmemory.h"

#define SEM_CONSUMER "/sem_consumer"
#define SEM_PRODUCER "/sem_producer"
#define path_to_shared_memory_data_sensor "./Memoire/data_capteur.mem"
#define path_to_my_shared_memory_increment "./Memoire/INCREMENT_S1.mem"
#define chemin_memoire_stable "memoire_stable.txt"
#define TAILLE_FENETRE 10
#define SLEEP_WATCHDOG 1

void* increment_watchdog_function(void*);

double mean(double*,int);

int main(int argc, char** argv){
    
    //initialisation des semaphores
    sem_unlink(SEM_CONSUMER);
    sem_unlink(SEM_PRODUCER);

    // Création des semaphores pour synchroniser le capteur (schema producteur/consommateur) 
    sem_t *sem_prod = sem_open(SEM_PRODUCER,O_CREAT,0660,1); //dernier 1 : valeur initialisation
    if (sem_prod == SEM_FAILED)
    {
      perror("sem_open/producer");
      exit(EXIT_FAILURE);
    }

    sem_t *sem_cons = sem_open(SEM_CONSUMER,O_CREAT,0660,0); //dernier 0 : valeur initialisation
    if (sem_cons == SEM_FAILED)
    {
      perror("sem_open/consumer");
      exit(EXIT_FAILURE);
    }

    // Accès à la mémoire partagée du capteur
    double* sensor_data;
    if((sensor_data=(double*)attach_memory_block(path_to_shared_memory_data_sensor, sizeof(double)))==NULL)
	  {
		  printf("erreur : capteur n'a pas acces au bloc \"Memoire/data_capteur.mem\"\n");
		  return -1;
	  }
    
    // Creation memoire variable watchdog et thread
    int watchdog_increment;
    pthread_t thread_increment;
    pthread_create(&thread_increment, NULL, increment_watchdog_function, (void*) &watchdog_increment);

    //lancement service
    printf("Hello, starting server 1\n");
    fflush(stdout);
    int i_fenetre = 0;
    double temperature[TAILLE_FENETRE]={0};
    double moyenne_glissante = 0.0;
    FILE* memoire_stable;
    while(1)
    { 
        sem_wait(sem_cons);
        temperature[i_fenetre] = *sensor_data;
        if ((memoire_stable = fopen(chemin_memoire_stable,"w"))== NULL)
        {
          perror("fopen:");
          exit(EXIT_FAILURE);
        }
        for(int i=0; i<TAILLE_FENETRE; i++){
          fprintf(memoire_stable, "%f\n", temperature[i]);
        }
        //derniere valeur :
        fprintf(memoire_stable,"---\n%d\n",i_fenetre);
        fclose(memoire_stable);
        printf("valeur t : %f\n",temperature[i_fenetre]);
        moyenne_glissante = mean(temperature, TAILLE_FENETRE);
        printf("moyenne : %f\n",moyenne_glissante);
        i_fenetre++;
        i_fenetre = i_fenetre%TAILLE_FENETRE;
        sleep(1);
        sem_post(sem_prod);
    }
    sem_close(sem_cons);
    sem_close(sem_prod);
    sem_unlink(SEM_CONSUMER);
    sem_unlink(SEM_PRODUCER);
}

// fonction appelée dans le Thread_increment pour indiquer au watchdog qu'on est en vie
void* increment_watchdog_function(void* p) {
  int* watchdog_increment = (int*) p;
  if((watchdog_increment = (int*)attach_memory_block(path_to_my_shared_memory_increment, sizeof(int)))==NULL)
    {
      printf("erreur : service 1 n'a pas acces au bloc \"Memoire/INCREMENT_S1.mem\"\n");
      return NULL;
    }
  (*watchdog_increment) = 0;
  while(1)
  {
    (*watchdog_increment)++;
    printf("watchdog : %d\n",*watchdog_increment);
    sleep(SLEEP_WATCHDOG);
  }
}

//fonction calcul de moyenne du tableau
double mean (double* temperature, int size){
    double somme = 0;
    for(int i=0; i<size; i++){
      somme += temperature[i];
    }
    return(somme/size);
}