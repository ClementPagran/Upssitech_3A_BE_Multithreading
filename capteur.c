#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include "sharedmemory.h"

int capteur()
{
	float* data;
	pid_t* pid_serv;
	pid_t* pid_capt;
	sigset_t   set;
	int sig;

	//Ajout du signal SIGUSR1 parmi ceux qui reveilleront le processus.
	sigemptyset(&set);                                                             
 	if(sigaddset(&set, SIGUSR1) == -1)
	{
		printf("Sigaddset error\n");                                                  
    	return -1;                                                    
	}                                                                              

	//Ecriture du pid du capteur dans le bloc memoire correspondant
	if((pid_capt = (pid_t*)attach_memory_block("./Memoire/PID_capteur.mem",sizeof(pid_t)))==NULL)
	{
		printf("erreur : capteur n'a pas acces au bloc \"Memoire/PID_capteur.mem\"\n");
		return -1;
	}
	*pid_capt = (int)getpid();
	
	//Recuperation du PID du service avec lequel le capteur communique
	if((pid_serv = (pid_t*)attach_memory_block("./Memoire/PID_S1.mem",sizeof(pid_t)))==NULL)
	{
		printf("erreur : capteur n'a pas acces au bloc \"Memoire/PID_S1.mem\"\n");
		return -1;
	}
	
	if((data = (float*)attach_memory_block("./Memoire/data_capteur.mem",sizeof(float)))==NULL)
	{
		printf("erreur : capteur n'a pas acces au bloc \"Memoire/data_capteur.mem\"\n");
		return -1;
	}
	
	while(1)
	{
		if(sigwait(&set,&sig)!=0)
		{
			printf("sigwait error\n");                                                  
			return -1;  
		}
		*data = random()%30; //genere la donnee (temperature aleatoire entre 0 et 29 degres)
		if(kill(*pid_serv,SIGUSR1) !=0)
		{
			printf("kill (signal vers service) error\n");
			return -1;
		}
	}
	detach_memory_block(data);
	detach_memory_block(pid_capt);
	detach_memory_block(pid_serv);
	return 0;
}