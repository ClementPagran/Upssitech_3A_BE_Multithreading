#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sharedmemory.h"

#define IPC_RESULT_ERROR (-1)

static int get_shared_block(char *nomfichier, int taille)
{
    key_t key; 
    key = ftok(nomfichier,0); //Convertir un nom de fichier et un identificateur de projet en clé IPC System V
    if (key == IPC_RESULT_ERROR)
    {
        return IPC_RESULT_ERROR;
    }
    return shmget(key,taille,0644|IPC_CREAT); //renvoie l'identifiant du segment de mémoire partagée
}


void * attach_memory_block(char *nomfichier,int taille)
{
    int shared_block_id = get_shared_block(nomfichier,taille);
    void *result;

    if (shared_block_id == IPC_RESULT_ERROR)
    {
        return NULL;
    }

    result = shmat(shared_block_id,NULL,0); //attache le segment de mémoire partagée System V identifié par shmid au segment de données du processus appelant
    if (result == (char *)IPC_RESULT_ERROR)
    {
        return NULL;
    }
    return result;
}

bool detach_memory_block(void *block)
{
    return (shmdt(block)!= IPC_RESULT_ERROR);
}

bool destroy_memory_block(char *nomfichier)
{
    int shared_block_id = get_shared_block(nomfichier, 0);

    if (shared_block_id == IPC_RESULT_ERROR)
    {
        return NULL;
    }
    return (shmctl(shared_block_id,IPC_RMID,NULL)!= IPC_RESULT_ERROR); // effectue  l'opération  de  contrôle  indiquée  par cmd sur le segment de mémoire partagée System V identifié par shmid.

}

// Test des fonctions
/*
int main()
{
    int* mem; //la memoire partagee va pointer sur un entier
    mem = (int*)attach_memory_block("sharedmem",sizeof(int)); // (le fichier sharedmem doit exister...) Crée et attache la memoire partagé
    *mem = 2; // Rempli la mémoire
    printf("%d\n",*mem);
    detach_memory_block(mem); // Detache la mémoire si plus besoin
    destroy_memory_block("sharedmem"); // Supprime la mémoire si personne en a plus besoin
}
*/

