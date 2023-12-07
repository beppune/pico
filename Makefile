
CC=gcc
CFLAGS=-Wall -Wpedantic

all: pico

pico: pico.o error.o
	$(CC) $? -o $@

pico.o: pico.c
	$(CC) $(CFLAGS) -c -o $@ $<

error.o: error.c error.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f pico *.o

r: pico
	./pico

e:
	${EDITOR} pico.c
