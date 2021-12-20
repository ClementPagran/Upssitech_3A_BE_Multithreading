#define SEM_CONSUMER "/sem_consumer"
#define SEM_PRODUCER "/sem_producer"
#define path_to_shared_memory_data_sensor "./Memoire/data_capteur.mem"
#define path_to_my_shared_memory_increment "./Memoire/INCREMENT_S1.mem"
#define path_to_PID_S2 "./Memoire/PID_S2.mem"
#define chemin_memoire_stable "./Memoire/memoire_stable.txt"
#define TAILLE_FENETRE 10
#define SLEEP_WATCHDOG 1

#ifndef _service2 // include guard
#define _service2
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
int sleeping;
void my_handler(int signum);
void to_sleep();
#endif