CC=gcc
FLAGS=-Wall -pthread

all: capteur calcul

capteur: capteur.c sharedmemory.o
	$(CC) $(FLAGS) $^ -o $@

calcul: calcul.c sharedmemory.o
	$(CC) $(FLAGS) $^ -o $@

%.o:%.c %.h
	$(CC) $(FLAGS) -c $<

make clean:
	rm -f *.o capteur calcul