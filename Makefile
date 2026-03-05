CC=gcc
CFLAGS=-Wall -Iinclude
LD=gcc
LFLAGS=-Lbin
AR=ar

SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,bin/%.o,$(SRC))
ARC=bin/libhalogen.a
BIN=bin/halogen
RUN=./$(BIN) test/test01.hal

.PHONY: all run

all: $(BIN)

run: all
	$(RUN)

clean:
	rm bin/*

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

$(BIN): $(ARC) bin/main.o
	$(LD) $(LFLAGS) -o $@ bin/main.o -lhalogen

$(ARC): $(filter-out bin/main.o,$(OBJ))
	$(AR) r $@ $^
