CC=gcc
FLAGS=-Wall -pthread
progs=capteur.exe service1.exe
dep=sharedmemory.o

all:$(progs)

%.exe: %.c $(dep)
	$(CC) $(FLAGS) $^ -o $@

%.o:%.c %.h
	$(CC) $(FLAGS) -c $<

make clean:
	rm -f *.o $(progs)