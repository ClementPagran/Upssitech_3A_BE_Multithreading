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
#include <string.h>

#include "sharedmemory.h"
#include "defines.h"
sigset_t sigset;
void* increment_watchdog_function(void*);

double mean(double*,int);

void read_from_stable_memory(double*,int*,FILE*);

void my_handler(int signum);

void to_sleep();

int main(int argc, char** argv){


    //Mise en veille de s2
    sleeping = 1;
	  signal(SIGUSR1, my_handler);
    
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);

    int* pid = (int*)attach_memory_block(path_to_PID_S2,sizeof(int));
    *pid = (int)getpid();
    printf("S2 : publishing PID : %d  \n", (int)getpid());
    to_sleep();
    

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
		  printf("erreur : service n'a pas acces au bloc %s\n",path_to_shared_memory_data_sensor);
		  return -1;
	  }
    
    // // Creation memoire variable watchdog et thread
    // int watchdog_increment;
    // pthread_t thread_increment;
    // pthread_create(&thread_increment, NULL, increment_watchdog_function, (void*) &watchdog_increment);

    //lancement service
    printf("Hello, starting backup server 2\n");
    //fflush(stdout);
    double moyenne_glissante = 0.0;
    FILE* memoire_stable;
    double temperature[TAILLE_FENETRE];
    int i_fenetre;
    if (argc == 2 && strcmp(argv[1],"resume")==0)
    {
      if ((memoire_stable = fopen(chemin_memoire_stable,"r"))== NULL)
        {
          perror("fopen:");
          exit(EXIT_FAILURE);
        }
      read_from_stable_memory(temperature,&i_fenetre,memoire_stable);
      fclose(memoire_stable);
      printf("reprise reussie !\n");
    }
    else
    {
      i_fenetre = 0;
    }



    while(1)
    { 
        sem_post(sem_prod);
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
        fprintf(memoire_stable,"---\n%d\n",i_fenetre); //indice en fin de fichier utile a la reprise
        fclose(memoire_stable);
        printf("valeur t : %f\n",temperature[i_fenetre]);
        moyenne_glissante = mean(temperature, TAILLE_FENETRE);
        printf("moyenne : %f\n",moyenne_glissante);
        i_fenetre++;
        i_fenetre = i_fenetre%TAILLE_FENETRE;
        sleep(1);
    }
    
    sem_close(sem_cons);
    sem_close(sem_prod);
    sem_unlink(SEM_CONSUMER);
    sem_unlink(SEM_PRODUCER);
}

void my_handler(int signum)
{
    if (signum == SIGUSR1)
    {
      printf("Received SIGUSR1!\n");
      //if we are sleeping
      if(sleeping)
      {
        sleeping = 0;
        sigprocmask(SIG_UNBLOCK, &sigset, NULL);
      }
      else
      {
        to_sleep();
      }
    }
}

void to_sleep()
{
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    /* Active waiting TODO: bad practice change this */
    //sigwait
    sleeping = 1;
    while(sleeping)
    {
        printf("main : pid %d with waiting for signals \n", (int)getpid());
        int signum;
        int result =sigwait(&sigset,&signum);
        if(result == 0)
        {
          printf("sigwait got signal: %d\n", signum);
          my_handler(signum);
          
        }
          
    }
}


void read_from_stable_memory(double* tableau, int* i_fenetre,FILE* memoire_stable)
{
  char chaine[100];
  for(int i = 0;i<TAILLE_FENETRE;i++)
  {
    fscanf(memoire_stable,"%s",chaine);
    //printf("%s\n",chaine);
    tableau[i] = strtod(chaine,NULL);
  }
  fscanf(memoire_stable,"%s",chaine);
  fscanf(memoire_stable,"%s",chaine);
  *i_fenetre = atoi(chaine)+1;  
}

// fonction appelée dans le Thread_increment pour indiquer au watchdog qu'on est en vie
void* increment_watchdog_function(void* p) {
  int* watchdog_increment = (int*) p;
  if((watchdog_increment = (int*)attach_memory_block(path_to_my_shared_memory_increment, sizeof(int)))==NULL)
    {
      printf("erreur : service 1 n'a pas acces au bloc %s\n",path_to_my_shared_memory_increment);
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