#include "safety.h"
#include "sharedmemory.h"
#include "defines.h"





void my_thread_func()
{
    while(1)
    {
        while(ticks*time_intervals_check <= timeout_sec)
        {
            printf("Watchdog waiting : %d\n",ticks*time_intervals_check);
            if(increment_prec!=*increment_jerem)
            {
                ticks = 0;
            }
            increment_prec = *increment_jerem;
            ticks++;
            sleep(time_intervals_check);
        }
        //S1 died
        ticks = 0;
        increment_prec  =0;
        *increment_jerem=0;

        start_backup_program();
        while(1)
        {   
            printf("\n Watchdog started S2 :\n");
            printf("Now waiting for S1 revival :\n");
            printf("> cd Upssitech_3A_BE_Multithreading\n");
            printf("> ./bin/service1.exe\n");
            printf("------------------------------------\n");

            if(increment_prec!=*increment_jerem)
            {
                break;
            }
            sleep(time_intervals_check);
        }
        //S1 is back 
        sleep_backup_program();

    }
}

void start_backup_program()
{
    printf("Starting backup\n");
    kill(pid_program2, SIGUSR1);
}

void sleep_backup_program()
{
    printf("Backup going to sleep\n");
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
    timeout_sec = 5;
    time_intervals_check = 1;
    my_thread_func();
}