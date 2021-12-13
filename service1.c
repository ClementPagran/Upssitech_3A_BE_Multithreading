#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include "sharedmemory.h"
#include <limits.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#define SEM_CONSUMER "/sem_consumer"
#define SEM_PRODUCER "/sem_producer"
#include <time.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

const char *path;
float* value_adr;
float value;
pthread_t thread_increment;
int* increment_test;

//char *path_to_my_shared_memory = "./Memoire/PID_S1.mem";
//char *path_to_my_shared_memory_increment = "./Memoire/INCREMENT_S1.mem";
char *path_to_shared_memory_data_sensor = "./Memoire/data_capteur.mem";

double mean (double* tab, int size){
    double temp = 0;
    for(int i=0; i<size; i++){
      temp += tab[i];
    }
    return(temp/size);
}

// fonction appelée dans le Thread_increment pour indiquer au watchdog qu'on est en vie

/*void *thread_increment_function() {
  for(int i=0; i < INT_MAX; i++){
    (*increment_test) = i;
    sleep(0.1);
  }
}
*/

int main(int argc, char** argv){

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

    printf("Hello, starting server 1\n");
    fflush(stdout);
    int cmp = 0;
    double tab[10] = {0};
    double moyenne_glissante = 0.0;
    moyenne_glissante = 1;

    // Accès à la mémoire partagée du capteur (1 *float)
    double* sensor_data;
    if((sensor_data=(double*)attach_memory_block("./Memoire/data_capteur.mem", sizeof(double)))==NULL)
	  {
		  printf("erreur : capteur n'a pas acces au bloc \"Memoire/data_capteur.mem\"\n");
		  return -1;
	  }

    // Creation memoire partagee PID pour chien de garde et le capteur
    //int* pid_adr = (int*)attach_memory_block(path_to_my_shared_memory, sizeof(int));
    //*pid_adr = (int)getpid();

    
    
    // Creation memoire Increment
    //increment_test = (int*)attach_memory_block(path_to_my_shared_memory_increment, sizeof(int));
    //pthread_create(&thread_increment, NULL, thread_increment_function, NULL);


    // Ajout des semaphores hakim ou signal pour indiquer au capteur de publier 
    while(1)
    {
        
        sem_wait(sem_cons);
        cmp = cmp%10;
        tab[cmp] = *sensor_data;
        printf("valeur t : %f\n",tab[cmp]);
        fflush(stdout);
        moyenne_glissante = mean(tab, 10);
        printf("moyenne : %f\n",moyenne_glissante);
        fflush(stdout);
        cmp++;
        sleep(1);
        sem_post(sem_prod);      
    }
    sem_close(sem_cons);
    sem_close(sem_prod);
    sem_unlink(SEM_CONSUMER);
    sem_unlink(SEM_PRODUCER);
}