#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <ctype.h>
#include <fcntl.h>

#include "error.h"
#include "input.h"
#include "editor_state.h"
#include "term.h"
#include "buffer.h"

struct termios orig_term;


#define ON_WINCH 0
struct sigaction actions[1];

void on_winch() {
	printf("Wind ow change\r\n");
}

struct editor_state ES;
struct buffer content = BUFFER_INIT;

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term);
	write(STDIN_FILENO, "\x1b[H\x1b[2J", 7);
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

void input(int r) {
	switch(r) {
		case 0x11: // CTRL-q. Quit without save
			exit(0);
			break;
		case 0x3: //CTRL-c. Save and quit
			exit(0);
			break;

		case ARROW_UP:
		case ARROW_DOWN:
		case ARROW_RIGHT:
		case ARROW_LEFT:
			move(STDIN_FILENO, r, &ES);
			break;
		case PAGE_UP:
			printf("PAGE_UP\r\n");
			break;
		case PAGE_DOWN:
			printf("PAGE_DOWN\r\n");
			break;
		case INSERT:
			printf("INSERT\r\n");
			break;
		case CANC:
			printf("CANC\r\n");
			break;
		case HOME:
			printf("HOME\r\n");
			break;
		case END:
			printf("END\r\n");
			break;
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

	init_state(&ES, ws.ws_col, ws.ws_row);

	// Set initial cursor position
	ES.cy = 6;
	ES.cx = 2;
	move_to(STDIN_FILENO, ES.cx, ES.cy);

	// Init status
	set_status_true(&ES, "pico editor. tty: %s, pid: %d. CTRL-q (quit), CTRL-c (save and quit)", ttyname(STDIN_FILENO), getpid());
	update_status(STDIN_FILENO, &ES);

	// Setup signal actions
	actions[ON_WINCH].sa_handler = on_winch;
	sigfillset(&actions[ON_WINCH].sa_mask);
	actions[ON_WINCH].sa_flags = SA_RESTART;
	sigaction(SIGWINCH, &actions[ON_WINCH], NULL);

}

void load_file(struct buffer *b);

void update_screen();

int main() {

	atexit(disableRawMode);

	enableRawMode();

	init();

	load_file(&content);

	update_screen();

	int c;
	do {
		c = getfrom(STDIN_FILENO);
		/*if( iscntrl(c) ) {
			printf("0x%x\r\n", c);
		}*/
		input(c);
	} while(1);

	return 0;
}

void load_file(struct buffer *b) {
	struct stat sample;
	stat("sample.txt", &sample);

	content.len = sample.st_size;
	content.ptr = realloc(content.ptr, content.len);

	ES.fd = open("sample.txt", O_RDWR);
	read(ES.fd, content.ptr, content.len);
}

void update_screen() {
	char *a, *r = content.ptr;
	while(*r!='\n') r++;
	int row = 1;
	while(row <= ES.height - 1) {
		move_to(ES.fd, row, 1);
		write(STDIN_FILENO, a, r-a);
		r++;
		a = r;
		while(*r != '\n')r++;
		row++;	
	}
}




