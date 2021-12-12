#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include "sharedmemory.h"
#include <limits.h>


const char *path;
float* value_adr;
float value;
pthread_t thread_increment;
int* increment_test; 
FIFO File;

typedef struct etCellule{
    float valelement;
    struct etCellule * suiv;
} Cellule;

typedef struct MA_FILE {
    Cellule * tete;
    Cellule * queue; 
} FIFO;
