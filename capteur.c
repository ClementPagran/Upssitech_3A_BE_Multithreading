#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include"sharedmemory.h"
#include<semaphore.h>
#include <sys/stat.h>
#include <fcntl.h> 
#define SEM_CONSUMER "/sem_consumer"
#define SEM_PRODUCER "/sem_producer"
#define SEM_CONSUMER "/sem_consumer"
#define SEM_PRODUCER "/sem_producer"

int main()
{
	float* data;
	if((data = (float*)attach_memory_block("./Memoire/data_capteur.mem",sizeof(float)))==NULL)
	{
		printf("erreur : capteur n'a pas acces au bloc \"Memoire/data_capteur.mem\"\n");
		return -1;
	}

	sem_t *sem_prod = sem_open(SEM_PRODUCER,1); //dernier 1 : valeur initialisation
	if (sem_prod == SEM_FAILED)
	{
		perror("sem_open/producer");
		exit(EXIT_FAILURE);
	}

	sem_t *sem_cons = sem_open(SEM_CONSUMER,0); //dernier 0 : valeur initialisation
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
		sem_post(sem_cons);
	}
	sem_close(sem_prod);
	sem_close(sem_cons);
	detach_memory_block(data);
	return 0;
}