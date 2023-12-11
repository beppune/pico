
#include "editor_state.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

void init_state(struct editor_state *es, int w, int h) {
	es->width = w;
	es->height = h;
	es->status = malloc(sizeof(char) * es->width);
	memset(es->status, 0x20, es->width);
}

void set_status(struct editor_state *es, const char *s, size_t len) {
	len = es->width < len ? es->width : len;
	memset(es->status, 0x20, es->width);
	strncpy(es->status, s, len);
}

void set_status_true(struct editor_state *es, const char *fmt, ...) {
	va_list arglist;
	va_start(arglist, fmt);
	vsnprintf(es->status, es->width, fmt, arglist);
	size_t n = strlen(es->status);
	va_end(arglist);

	if( n < es->width ) {
		char *p = es->status + n;
		memset(p, 0x20, es->width - n);
	}

}
