#ifndef EDITOR_STATE_H
#define EDITOR_STATE_H

#include <stddef.h>

struct editor_state {
	unsigned int width;
	unsigned int height;
	unsigned int cx;
	unsigned int cy;
	char *status;
	int fd;
};

void init_state(struct editor_state *es, int w, int h);

void set_status(struct editor_state *es, const char *s, size_t len);

void set_status_true(struct editor_state *es, const char *fmt, ...);

#endif // EDITOR_STATE_H
