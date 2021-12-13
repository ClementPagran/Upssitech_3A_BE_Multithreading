#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>

#ifndef _sharemem // include guard
#define __sharemem

#define IPC_RESULT_ERROR (-1)

static int get_shared_block(char *nomfichier, int taille)
{
    key_t key;
    key = ftok(nomfichier,0);
    if (key == IPC_RESULT_ERROR)
    {
        return IPC_RESULT_ERROR;
    }
    return shmget(key,taille,0644|IPC_CREAT);
}

void * attach_memory_block(char *nomfichier,int taille)
{
    int shared_block_id = get_shared_block(nomfichier,taille);
    void *result;

    if (shared_block_id == IPC_RESULT_ERROR)
    {
        return NULL;
    }

    result = shmat(shared_block_id,NULL,0);
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
    return (shmctl(shared_block_id,IPC_RMID,NULL)!= IPC_RESULT_ERROR);
}
#endif