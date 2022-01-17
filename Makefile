COMP=gcc
FLAGS=-Wall -pthread
SRC=src
OBJ=obj
BIN=bin
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
targets=capteur service1 watchdog service2
BINS=$(patsubst  %,$(BIN)/%.exe,$(targets))
DEP=sharedmemory
DEPS=$(patsubst  %,$(OBJ)/%.o,$(DEP))

all:$(BINS)
	echo $(BINS)

$(OBJ)/%.o:$(SRC)/%.c
	mkdir -p obj
	$(COMP) $(FLAGS) -c $^ -o $@

$(BIN)/%.exe: $(OBJ)/%.o $(DEPS)
	mkdir -p bin
	$(COMP) $(FLAGS) $^ -o $@

clean:
	rm -f $(BIN)/* $(OBJ)/*