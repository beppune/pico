
#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

struct buffer {
	char *ptr;
	size_t len;
};

#define BUFFER_INIT { NULL, 0 }

void append_buffer(struct buffer *bf, const char *str, size_t len);

void free_buffer(struct buffer *bf);

#endif // BUFFER_H

