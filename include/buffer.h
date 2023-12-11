#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

struct buffer{
    char *b;
    size_t len;
}

#define NEW_BUFFER { NULL, O }

void buffer_append(struct buffer * bf, const char *str, size_t len);

void buffer_free(struct buffer *bf) {

#endif // BUFFER_H
