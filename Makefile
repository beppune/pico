
CC=gcc
CFLAGS=-Wall -Wpedantic

all: pico

pico: pico.o input.o error.o
	$(CC) $^ -o $@

pico.o: pico.c 
	$(CC) $(CFLAGS) -c -o $@ $<

input.o: input.c
	$(CC) $(CFLAGS) -c -o $@ $<

error.o: error.c 
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f pico *.o

r: pico
	./pico

