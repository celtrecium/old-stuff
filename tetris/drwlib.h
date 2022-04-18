#ifndef DRWLIB_H
#define DRWLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/syscall.h>
#include "celtlib.h"

void dl_puts(const char *string);
void set_colour(unsigned int bg, unsigned int fg);
void set_position(int x_pos, int y_pos);
void clear();
void printxy(int x_pos, int y_pos, char symbol);
void draw_rectangle(int x0_pos, int y0_pos, int x1_pos, int y1_pos, char symbol);
void draw_line(int x1, int y1, int x2, int y2, char symbol);
void draw_box(int x0_pos, int y0_pos, int x1_pos, int y1_pos, int bg0, int fg0, int bg1, int fg1,
        char *ch, char *ch_1, char *ch_2, char *ch_3, char *ch_4, char *ch_5, char *ch_6, char *ch_7);

#endif /* DRWLIB_H */
