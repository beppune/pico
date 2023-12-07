#ifndef INPUT_H
#define INPUT_H

/**
 * Input
 */

enum KEY {
	ARROW_UP	= 1000,
	ARROW_DOWN,
	ARROW_RIGHT,
	ARROW_LEFT
};

/*
 * Get a single character from given fd
 * and return the retrieved character.
 * If <ESCAPE> is detected it reads other
 * characters and return a KEY enum constant
 * based on the escape sequence red.
 * It might read other charatcters
 */
int getfrom(int fd);

#endif //INPUT_H

