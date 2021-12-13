#include "prog2.h"
#include "sharedmemory.h"

void my_handler(int signum)
{
    if (signum == SIGUSR1)
    {
        printf("Received SIGUSR1!\n");
        sleeping = 0;
    }
}
void to_sleep()
{
    /* Active waiting TODO: bad practice change this */
    //sigwait
    sleeping = 1;
    while(sleeping)
    {
        
        printf("main : pid %d with waiting for signals \n", (int)getpid());
        sleep(3);
    }
}
int main (int argc, char *argv []) 
{
    sleeping = 1;
	signal(SIGUSR1, my_handler);
    int* pid = (int*)attach_memory_block("/home/ugo/Desktop/Upssitech_3A_BE_Multithreading/src/PID_S2.mem",sizeof(int));
    *pid = (int)getpid();
    printf("S2 : publishing PID : %d  \n", (int)getpid());
    to_sleep();
}