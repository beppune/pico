#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <signal.h>

struct termios orig_term;


#define ON_WINCH 0
struct sigaction actions[1];

void on_winch() {
	printf("Wind ow change\r\n");
}

struct editor_state {
	unsigned int width;
	unsigned int height;
} ES;

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

char readbuf[5];

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
	if( !isatty(STDIN_FILENO) ) die("not a tty");

	// Set input buffer to zeroes
	memset(readbuf, 0, 5);

	// Clear screen
	write(STDIN_FILENO, "\x1b[2J", 4);

	// Move Cursor upper-left
	write(STDIN_FILENO, "\x1b[H", 3);

	// Get terminal size
	struct winsize ws;
	if( ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1 )
		die("ioctl");

	ES.width = ws.ws_col;
	ES.height = ws.ws_row;
	printf("tty: %s, pid: %d, w: %d, h: %d\r\n", ttyname(STDIN_FILENO), getpid(),  ES.width, ES.height);

	// Setup gsignal actions
	actions[ON_WINCH].sa_handler = on_winch;
	sigfillset(&actions[ON_WINCH].sa_mask);
	actions[ON_WINCH].sa_flags = SA_RESTART;
	sigaction(SIGWINCH, &actions[ON_WINCH], NULL);
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
