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

const char *path;
float* value_adr;
float value;

void* thread_increment_function(void* p);

//char *path_to_my_shared_memory = "./Memoire/PID_S1.mem";

double mean (double* tab, int size){
    double temp = 0;
    for(int i=0; i<size; i++){
      temp += tab[i];
    }
    return(temp/size);
}

// fonction appelée dans le Thread_increment pour indiquer au watchdog qu'on est en vie

int* watchdog_increment;


int main(int argc, char** argv){
    
    pthread_t thread_increment;

    sem_unlink(SEM_CONSUMER);
    sem_unlink(SEM_PRODUCER);

    // Création des semaphores 
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

  

    // Accès à la mémoire partagée du capteur (1 *float)
    double* sensor_data;
    if((sensor_data=(double*)attach_memory_block(path_to_shared_memory_data_sensor, sizeof(double)))==NULL)
	  {
		  printf("erreur : capteur n'a pas acces au bloc \"Memoire/data_capteur.mem\"\n");
		  return -1;
	  }

    // Creation memoire partagee PID pour chien de garde et le capteur
    //int* pid_watchdog = (int*)attach_memory_block(path_to_my_shared_memory_increment, sizeof(int));

    //*pid_adr = (int)getpid();

    
    
    // Creation memoire Increment
    if((watchdog_increment = (int*)attach_memory_block(path_to_my_shared_memory_increment, sizeof(int)))==NULL)
    {
      printf("erreur : service 1 n'a pas acces au bloc \"Memoire/INCREMENT_S1.mem\"\n");
      return -1;
    }
    pthread_create(&thread_increment, NULL, thread_increment_function, NULL);
    printf("Hello, starting server 1\n");
    fflush(stdout);
    int cmp = 0;
    double tab[TAILLE_FENETRE] = {0};
    double moyenne_glissante = 0.0;
    int x = 0;
    while(1)
    {
        FILE* memoire_stable = fopen(chemin_memoire_stable,"w");
        if (memoire_stable == NULL)
        {
          perror("fopen:");
          exit(EXIT_FAILURE);
        }
        sem_wait(sem_cons);
        cmp = cmp%TAILLE_FENETRE;
        tab[cmp] = *sensor_data;
        //fprintf(memoire_stable,"%f\n",tab[cmp]);
        //sprintf(s,"%f",tab[cmp]);
        //fputs("1\n",memoire_stable);
        //fputs(s,memoire_stable);
        //fputs("\n",memoire_stable);
        for(int i=0; i<10; i++){
          fprintf(memoire_stable, "%f\n", tab[i]);
        }
        printf("valeur t : %f\n",tab[cmp]);
        fflush(stdout);
        moyenne_glissante = mean(tab, TAILLE_FENETRE);
        printf("moyenne : %f\n",moyenne_glissante);
        fflush(stdout);
        cmp++;
        sleep(1);
        sem_post(sem_prod);  
        fclose(memoire_stable);    
    }
    sem_close(sem_cons);
    sem_close(sem_prod);
    sem_unlink(SEM_CONSUMER);
    sem_unlink(SEM_PRODUCER);
    
}

void* thread_increment_function(void* p) {
  for(int i=0; i < INT_MAX; i++){
    (*watchdog_increment) = i;
    printf("watchdog : %d\n",*watchdog_increment);
    sleep(1);
  }
}