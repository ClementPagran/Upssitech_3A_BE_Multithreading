#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "sharedmemory.h"
#include "defines.h"

void start_backup_program(pid_t pid_s2)
{
    printf("Starting backup\n");
    kill(pid_s2, SIGUSR1);
}

void sleep_backup_program(pid_t pid_s2)
{
    printf("Backup going to sleep\n");
    kill(pid_s2, SIGUSR1);
}

int main(int argc, char *argv[])
{
    int ticks = 1;
    int increment_precedent = 0;
    pid_t *pid_s2 = (pid_t *)attach_memory_block(path_to_PID_S2, sizeof(pid_t));
    if (pid_s2 == NULL)
    {
        printf("erreur : watchdog n'a pas acces au bloc %s\n", path_to_PID_S2);
        return -1;
    }
    int *increment_courant = (int *)attach_memory_block(path_to_my_shared_memory_increment, sizeof(int));
    if (increment_courant == NULL)
    {
        printf("erreur : watchdog n'a pas acces au bloc %s\n", path_to_my_shared_memory_increment);
        return -1;
    }

    while (1)
    {
        while (ticks * TIME_INTERVALS_CHECK <= TIME_OUT_WATCHDOG)
        {
            printf("Watchdog waiting : %d\n", ticks * TIME_INTERVALS_CHECK);
            if (increment_precedent != *increment_courant)
            {
                ticks = 0;
            }
            increment_precedent = *increment_courant;
            ticks++;
            sleep(TIME_INTERVALS_CHECK);
        }
        // S1 died
        ticks = 0;
        // increment_precedent=0;

        start_backup_program(*pid_s2);
        while (1)
        {
            printf("\n Watchdog started S2 :\n");
            printf("Now waiting for S1 revival :\n");
            if (increment_precedent != *increment_courant)
            {
                // le compteur a change,
                break;
            }
            sleep(TIME_INTERVALS_CHECK);
        }
        // S1 is back
        sleep_backup_program(*pid_s2);
    }
}