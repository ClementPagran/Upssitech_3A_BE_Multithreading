#ifndef _safety // include guard
#define _safety
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
int system1_is_dead;

int ticks;
int time_intervals_check;
int timeout_ms;

int pid_program2;

int* increment_jerem;
int increment_prec;

void my_thread_func();
void start_backup_program();
int get_pid();

#endif