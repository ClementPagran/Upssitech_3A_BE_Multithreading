#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H
#include <stdbool.h>

void * attach_memory_block(char *nomfichier,int taille); //(cree et )attache la memoire partagee, nomfichier doit exister, taille pour 3 floats : 3*sizeof(float)
bool detach_memory_block(void *block); //detache la memoire partagee
bool destroy_memory_block(char *nomfichier); //supprime la memoire partagee (a appeler si plus de processus attache a cette memoire)

#endif
