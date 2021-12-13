#include "safety.h"
#include "sharedmemory.h"

void my_thread_func()
{
    while(ticks*time_intervals_check <= timeout_ms)
    {
        if(increment_prec!=*increment_jerem)
        {
            ticks = 0;
        }
        increment_prec = *increment_jerem;
        ticks++;
        sleep(((float)time_intervals_check)/1000.0);
    }
    start_backup_program();
}

void start_backup_program()
{
    kill(pid_program2, SIGUSR1);
}

int get_pid()
{
    int* pid = (int*)attach_memory_block("/home/ugo/Desktop/Upssitech_3A_BE_Multithreading/src/PID_S2.mem",sizeof(int));
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
    increment_jerem = (int*)attach_memory_block("/home/ugo/Desktop/Upssitech_3A_BE_Multithreading/src/INCREMENT_S1.mem",sizeof(int));
    ticks = 1;
    timeout_ms = 50000;
    time_intervals_check = 200;
    my_thread_func();
}