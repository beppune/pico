
CC=gcc
CFLAGS=-Wall -Wpedantic

all: pico

pico: pico.o term.o input.o error.o editor_state.o
	$(CC) $^ -o $@

pico.o: pico.c 
	$(CC) $(CFLAGS) -c -o $@ $<

term.o: term.c 
	$(CC) $(CFLAGS) -c -o $@ $<

input.o: input.c
	$(CC) $(CFLAGS) -c -o $@ $<

error.o: error.c 
	$(CC) $(CFLAGS) -c -o $@ $<

editor_state.o: editor_state.c 
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f pico *.o

r: pico
	./pico

