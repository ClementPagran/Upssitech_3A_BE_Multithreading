#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <stdbool.h>

void * attach_memory_block(char *nomfichier,int taille);
bool detach_memory_block(void *block);
bool destroy_memory_block(char *nomfichier);

#endif