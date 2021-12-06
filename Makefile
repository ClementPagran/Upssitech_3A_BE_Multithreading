COMP=gcc
FLAGS=-Wall

all: capteur

%.o: %.c %.h
	$(COMP) $(FLAGS) -c $<

capteur: capteur.c sharedmemory.o
	$(COMP) $(FLAGS) -o $@ $^

clean:
	rm -f *.o *.gch capteur