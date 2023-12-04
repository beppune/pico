#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_term;

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term);
}

void enableRawMode() {
	tcgetattr(STDIN_FILENO, &orig_term);
	atexit(disableRawMode);

	struct termios raw = orig_term;
	raw.c_lflag &= ~(ECHO | ICANON);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {

	atexit(disableRawMode);

	enableRawMode();

	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 ) {
		if (iscntrl(c)) {
			printf("%d, %x\n", c, c);
		} else {
			printf("%d, '%c', %x\n", c, c, c);
		}
	}

	return 0;
}
