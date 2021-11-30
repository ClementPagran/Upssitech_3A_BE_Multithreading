#include<stdio.h>
#include<stdlib.h>
#include "sharedmemory.h"

int main()
{
	float* mem;
	if(mem = (float*)attach_memory_block("./Memoire/data_capteur.mem",sizeof(float))== NULL)
	{
		printf("erreur : capteur n'a pas acces au bloc \"Memoire/data_capteur.mem\"\n");
		return -1;
	}
	*mem = random()%30; //genere la donnee (temperature aleatoire entre 0 et 29 degres)
	//detach_memory_block(mem);
    //destroy_memory_block("sharedmem");
}
