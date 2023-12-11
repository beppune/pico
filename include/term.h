#ifndef TERM_H
#define TERM_H

#include "editor_state.h"

void hide_cursor(int fd);

void show_cursor(int fd);

void move_to(int fd, int x, int y);

void move(int fd, int dir, struct editor_state *es);

void update_status(int fd, struct editor_state *es);

#endif // TERM_H
