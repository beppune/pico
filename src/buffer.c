#include "buffer.h"

#include <string.h>
#include <stdlib.h>

void buffer_append(struct buffer * bf, const char *str, size_t len) {
    if( bf == NULL ) return;

    bf->p = realloc(bf->p, sizeof(char) * len);

    char *p = bf->p + bf->len;

    memcpy(p, str, len);

    bf->len += len;

}

void buffer_free(struct buffer *bf) {
    if( bf == NULL ) return;

    free(bf->p);
    bf->p = NULL;
    bf->len = 0;
}
