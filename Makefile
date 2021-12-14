CC=gcc
FLAGS=-Wall -pthread
SRC=src
OBJ=obj
BIN=bin
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
targets=capteur service1
BINS=$(patsubst  %,$(BIN)/%.exe,$(targets))
DEP=sharedmemory
DEPS=$(patsubst  %,$(OBJ)/%.o,$(DEP))

all:$(BINS)

$(OBJ)/%.o:$(SRC)/%.c
	$(CC) $(FLAGS) -c $^ -o $@

$(BIN)/%.exe: $(OBJ)/%.o $(DEPS)
	$(CC) $(FLAGS) $^ -o $@

make clean:
	rm -f $(BIN)/* $(OBJ)/*