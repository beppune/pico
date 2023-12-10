#include "input.h"
#include "error.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MAX_READBUF 5
char read_buf[MAX_READBUF];

int parse_escape() {
	const char * b = read_buf;
	if( *b != '[') die("not an escape sequence");
	if( strncmp(b, "[A", 2) == 0 ) return ARROW_UP;
	if( strncmp(b, "[B", 2) == 0 ) return ARROW_DOWN;
	if( strncmp(b, "[C", 2) == 0 ) return ARROW_RIGHT;
	if( strncmp(b, "[D", 2) == 0 ) return ARROW_LEFT;
	if( strncmp(b, "[5~", 3) == 0 ) return PAGE_UP;
	if( strncmp(b, "[6~", 3) == 0 ) return PAGE_DOWN;
	if( strncmp(b, "[2~", 3) == 0 ) return INSERT;
	if( strncmp(b, "[3~", 3) == 0 ) return CANC;
	if( strncmp(b, "[H", 2) == 0 ) return HOME;
	if( strncmp(b, "[F", 2) == 0 ) return END;


	printf("%s\r\n", read_buf);

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
			red = parse_escape();
			if( red != 0 ) {
				return red;
			}
		default:
			return r;
	}
}
