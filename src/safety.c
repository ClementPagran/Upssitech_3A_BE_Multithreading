#include "safety.h"
#include "sharedmemory.h"
#include "defines.h"

void my_thread_func()
{
    int cpt=0;
    while(ticks*time_intervals_check <= timeout_ms)
    {
        printf("Boucle:%d | %d\n",cpt,ticks*time_intervals_check);
        if(increment_prec!=*increment_jerem)
        {
            ticks = 0;
        }
        increment_prec = *increment_jerem;
        cpt++;
        ticks++;
        sleep(1);
    }
    start_backup_program();
}

void start_backup_program()
{
    printf("Signal Send\n");
    kill(pid_program2, SIGUSR1);
}

int get_pid()
{
    int* pid = (int*)attach_memory_block(path_to_PID_S2,sizeof(int));
    while(!pid)
    {
        printf("  waiting for pid \n");
        sleep(3);
    }
    printf("=> PID of S2 found : %d \n", *pid);
    return *pid;  
}


int main (int argc, char *argv []) 
{
    pid_program2 = get_pid();
    increment_jerem = (int*)attach_memory_block(path_to_my_shared_memory_increment,sizeof(int));
    ticks = 1;
    timeout_ms = 1000;
    time_intervals_check = 200;
    my_thread_func();
}