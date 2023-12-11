#include "buffer.h"

#include <stdlib.h>
#include <string.h>

void append_buffer(struct buffer *bf, const char *str, size_t len) {
	bf->ptr = realloc(bf->ptr, sizeof(char) * (bf->len + len) );

	char *p = bf->ptr + bf->len;
	memcpy(p, str, len);

	bf->len += len;
}

void free_buffer(struct buffer *bf) {
	free(bf->ptr);
	bf->len = 0;
	bf->ptr = NULL;
}
