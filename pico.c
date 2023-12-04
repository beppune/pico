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
	raw.c_lflag &= ~(ECHO | ICANON);
	
	raw.c_iflag &= ~(IXON);
	
	raw.c_oflag &= ~(OPOST);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void init() {
	struct stat s;
	if( fstat(STDIN_FILENO, &s) == -1) die("init");

	if(  S_ISFIFO(s.st_mode) ) die("stdin_pipe");

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
		if (iscntrl(c)) {
			printf("%d, 0x%x\r\n", c, c);
		} else {
			printf("%d, '%c', 0x%x\r\n", c, c, c);
		}
	}

	return 0;
}
