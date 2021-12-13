#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include "sharedmemory.h"
#include <limits.h>
#include <stdbool.h>
#include<sys/stat.h>
#include<fcntl.h>
#define SEM_CONSUMER "/sem_consumer"
#define SEM_PRODUCER "/sem_producer"
#include<time.h>
#include<semaphore.h>

#include<stdio.h>
#include<stdlib.h>

const char *path;
float* value_adr;
float value;
pthread_t thread_increment;
int* increment_test; 

