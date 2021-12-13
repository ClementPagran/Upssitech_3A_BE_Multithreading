#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include "sharedmemory.h"
#include <limits.h>
#include <stdbool.h>

const char *path;
float* value_adr;
float value;
pthread_t thread_increment;
int* increment_test; 

