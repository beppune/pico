
pico: pico.c
	@gcc -Wall -Wpedantic pico.c -o pico

r: pico
	./pico

e:
	${EDITOR} pico.c
