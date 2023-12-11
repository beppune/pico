#include "error.h"

#include <stdio.h>
#include <stdlib.h>

void die(const char *topic) {
	perror(topic);
	exit(1);
}

