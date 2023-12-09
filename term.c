#include "term.h"
#include "input.h"
#include "editor_state.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>

void hide_cursor(int fd) {
	write(fd, "\x1b[?25l", 6);
}

void show_cursor(int fd) {
	write(fd, "\x1b[?25h", 6);
}

void move_to(int fd, int x, int y) {
	char b[10];
	memset(b, 0, sizeof(char)*10);
	sprintf(b, "\x1b[%d;%dH", y, x);
	write(fd, b, strlen(b));
}

void move(int fd, int dir, struct editor_state *es) {
	char b[] = { '\x1b', '[', 'A' };
	switch(dir) {
		case ARROW_UP:
			if( es->cy == 1 ) return; 
			b[2] = 'A';
			es->cy -= 1;
			break;
		case ARROW_DOWN:
			if( es->cy == es->height ) return;
			b[2] = 'B';
			es->cy += 1;
			break;
		case ARROW_RIGHT:
			if( es->cx == es->width ) return;
			b[2] = 'C';
			es->cx += 1;
			break;
		case ARROW_LEFT:
			if( es->cx == 1 ) return;
			b[2] = 'D';
			es->cx -= 1;
			break;
		default:
			return;
	}
	write(fd, b, 3);
}

void update_status(int fd, struct editor_state * es) {
	write(fd, "\x1b[?25l", 6);
	move_to(fd, 1, es->height + 1);

	write(fd, "\x1b[1m\x1b[7m", 8);
	write(fd, es->status, es->width);
	write(fd, "\x1b[m", 3);

	move_to(fd, es->cx, es->cy);
	write(fd, "\x1b[?25h", 6);
}

