#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

struct termios orig_term;

void die(const char *topic) {
	perror(topic);
	exit(1);
}

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term);
}

void enableRawMode() {
	tcgetattr(STDIN_FILENO, &orig_term);
	atexit(disableRawMode);

	struct termios raw = orig_term;
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

	raw.c_iflag &= ~(BRKINT | IXON | ICRNL | IGNBRK | IGNCR | INLCR | INPCK | ISTRIP | PARMRK);

	raw.c_oflag &= ~(OPOST);

	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void input(char r) {
	switch(r) {
		case 0x11:
			printf("pico: Bye, Bye!\r\n");
			exit(0);
			break;
		case 0x3:
			printf("pico: Bye, Bye! (saving buffer)\r\n");
			exit(0);
			break;
	}

	//UP		0x1b[A
	//DOWN		0x1b[B
	//RIGHT		0x1b[C
	//LEFT		0x1b[D

	if (iscntrl(r)) {
		printf("%d, 0x%x\r\n", r, r);
	} else {
		printf("%d, '%c', 0x%x\r\n", r, r, r);
	}
}

void init() {
	struct stat s;
	if( fstat(STDIN_FILENO, &s) == -1) die("init");

	if( S_ISFIFO(s.st_mode) ) die("stdin_pipe");

	// Clear screen
	write(STDIN_FILENO, "\x1b[2J", 4);

	// Move Cursor upper-left
	write(STDIN_FILENO, "\x1b[H", 3);
}

int main() {

	atexit(disableRawMode);

	enableRawMode();

	init();

	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 ) {
		input(c);
	}

	printf("pico: bye, bye!\r\n");
	return 0;
}
