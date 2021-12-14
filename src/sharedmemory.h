#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H
#include <stdbool.h>

//########################################
//Crée et attache une mémoire partagée a un fichier avec un taille donné
//Input:
//  char *nomfichier : path vers un fichier qui va etre la mémoire partagé
//  int taille : taille de cette mémoire (3*sizeof(float) pour une taille de 3 float)
//Output:
//  void * : pointeur vers le mémoire créé
//########################################
void * attach_memory_block(char *nomfichier,int taille);

//########################################
//Detache la mémoire du processus
//Input:
//  void *block : memoire a détacher
//Output:
//  bool : boolean si operation réussi
//########################################
bool detach_memory_block(void *block);

//########################################
//Supprime la mémoire partagée
//Input:
//  char * : nom de fichier utilisé pour créer la mémoire
//Output:
//  bool : boolean si operation réussi
//########################################
bool destroy_memory_block(char *nomfichier);

#endif