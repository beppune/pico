#include "input.h"
#include "error.h"

#include <unistd.h>
#include <stdio.h>

#define MAX_READBUF 5
char read_buf[MAX_READBUF];

int parse_escape(const char *b, size_t size) {
	if( *b != '[') die("not an escape sequence");
	b++;
	while(b != b + size) {

		// Test for arrow keys
		switch( *b  ) {
			case 'A':
				return ARROW_UP;
			case 'B':
				return ARROW_DOWN;
			case 'C':
				return ARROW_RIGHT;
			case 'D':
				return ARROW_LEFT;
		}

		b++;
	}

	return 0;
}

/* Refer to enum KEY in input.h */
int getfrom(int fd) {
	char r;
	size_t red = read(fd, &r, 1);
	if( -1 == red ) die("getfrom");

	switch(r) {
		case 0x1b:
			red = read(fd, read_buf, MAX_READBUF);
			if( -1 == red ) die("getfrom");
			return parse_escape(read_buf, MAX_READBUF);
			break;
		default:
			return r;
	}
}
