#include<stdio.h>
#include<stdlib.h>
#include "sharedmemory.h"

int main()
{
	float* mem;
    mem = (float*)attach_memory_block("Memoire/data_capteur.mem",sizeof(float));
	*mem = random()%30; //genere la donnee (temperature aleatoire entre 0 et 29 degres)
	//detach_memory_block(mem);
    //destroy_memory_block("sharedmem");
}
