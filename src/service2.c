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
#include "service.h"

sigset_t sigset;

void my_handler();

static int sleeping;

static int faire_reprise;

int main(int argc, char **argv)
{
  signal(SIGUSR1, my_handler);
  sigemptyset(&sigset);
  if (sigaddset(&sigset, SIGUSR1) == -1)
  {
    printf("sigaddset error\n");
    return -1;
  }

  pid_t *pid_s2 = (pid_t *)attach_memory_block(path_to_PID_S2, sizeof(pid_t));
  *pid_s2 = getpid();
  printf("S2 : publishing PID : %d \n", (int)getpid());

  // initialement S2 dort :
  sleeping = 0;
  kill(getpid(), SIGUSR1);

  sem_t *sem_prod;
  sem_t *sem_cons;
  double moyenne_glissante = 0.0;
  FILE *memoire_stable;
  double temperature[TAILLE_FENETRE];
  int i_fenetre;
  double *sensor_data;

  // Accès à la mémoire partagée du capteur
  if ((sensor_data = (double *)attach_memory_block(path_to_shared_memory_data_sensor, sizeof(double))) == NULL)
  {
    printf("erreur : service n'a pas acces au bloc %s\n", path_to_shared_memory_data_sensor);
    return -1;
  }

  sem_prod = sem_open(SEM_PRODUCER, 0);
  if (sem_prod == SEM_FAILED)
  {
    perror("sem_open/producer: ");
    exit(EXIT_FAILURE);
  }

  sem_cons = sem_open(SEM_CONSUMER, 0);
  if (sem_cons == SEM_FAILED)
  {
    perror("sem_open/consumer: ");
    exit(EXIT_FAILURE);
  }

  sem_post(sem_prod); // Pour permettre la reprise (voir schema semaphores)
  faire_reprise = 1;
  while (1)
  {
    sem_wait(sem_cons);

    if (faire_reprise == 1) // S2 ne meurt jamais -> faire reprise a chaque fois
    {
      if ((memoire_stable = fopen(chemin_memoire_stable, "r")) == NULL)
      {
        perror("fopen: ");
        exit(EXIT_FAILURE);
      }
      read_from_stable_memory(temperature, &i_fenetre, memoire_stable);
      fclose(memoire_stable);
      printf("reprise reussie !\n");
      faire_reprise = 0;
    }

    temperature[i_fenetre] = *sensor_data;
    if ((memoire_stable = fopen(chemin_memoire_stable, "w")) == NULL)
    {
      perror("fopen: ");
      exit(EXIT_FAILURE);
    }
    for (int i = 0; i < TAILLE_FENETRE; i++)
    {
      fprintf(memoire_stable, "%f\n", temperature[i]);
    }
    fprintf(memoire_stable, "---\n%d\n", i_fenetre); // indice en fin de fichier utile a la reprise
    fclose(memoire_stable);
    printf("valeur t : %f\n", temperature[i_fenetre]);
    moyenne_glissante = mean(temperature, TAILLE_FENETRE);
    printf("moyenne : %f\n", moyenne_glissante);
    i_fenetre++;
    i_fenetre %= TAILLE_FENETRE;
    sleep(SLEEP_SERVICE);

    sem_post(sem_prod);
  }

  sem_close(sem_cons);
  sem_close(sem_prod);
  sem_unlink(SEM_CONSUMER);
  sem_unlink(SEM_PRODUCER);
}

void my_handler()
{
  //  cas ou S2 est actif
  if (sleeping == 0) // on endort
  {
    sleeping = 1;
    int signum;
    int result = sigwait(&sigset, &signum); // attente
    if (result != 0)
    {
      printf("sigwait error\n");
      exit(EXIT_FAILURE);
    }
    sleeping = 0;
  }

  // cas ou S2 dort
  else // on reveille
  {
    sleeping = 0;
  }
  faire_reprise = 1;
}