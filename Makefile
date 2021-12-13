CC=gcc
FLAGS=-Wall -pthread
progs=capteur service1
dep=sharedmemory.o

all:$(progs)

capteur: capteur.c $(dep)
	$(CC) $(FLAGS) $^ -o $@

service1: service1.c $(dep)
	$(CC) $(FLAGS) $^ -o $@

%.o:%.c %.h
	$(CC) $(FLAGS) -c $<

make clean:
	rm -f *.o $(progs)