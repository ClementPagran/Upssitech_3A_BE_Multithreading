#ifndef _prog2 // include guard
#define _prog2
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
int sleeping;
void my_handler(int signum);
void to_sleep();
#endif


