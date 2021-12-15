#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

#include"sharedmemory.h"
#include "defines.h"


int main()
{
	double* data;
	if((data = (double*)attach_memory_block(path_to_shared_memory_data_sensor,sizeof(double)))==NULL)
	{
		printf("erreur : capteur n'a pas acces au bloc \"Memoire/data_capteur.mem\"\n");
		return -1;
	}

	sem_t *sem_prod = sem_open(SEM_PRODUCER,0);
	if (sem_prod == SEM_FAILED)
	{
		perror("sem_open/producer");
		exit(EXIT_FAILURE);
	}

	sem_t *sem_cons = sem_open(SEM_CONSUMER,0);
	if (sem_cons == SEM_FAILED)
	{
		perror("sem_open/consumer");
		exit(EXIT_FAILURE);
	}
	srand(time(NULL)); //donnée aleatoire a chaque lancement du programme
	while(1)
	{
		sem_wait(sem_prod);
		*data = 0.1*(random()%300); //genere la donnee (temperature aleatoire entre 0 et 29,9 degres)
		printf("valeur capteur : %f\n",*data);
		fflush(stdout);
		sem_post(sem_cons);
	}
	sem_close(sem_prod);
	sem_close(sem_cons);
	detach_memory_block(data);
	return 0;
}