#ifndef ERROR_H
#define ERROR_H

#include <errno.h>

/*
 * Print perror with the given topic
 * 	and calls exit() with status 1
 */
void die(const char *topic);

#endif // ERROR_H

