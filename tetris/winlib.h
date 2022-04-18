#ifndef WINLIB_H
#define WINLIB_H

#include <stdio.h>
#include "drwlib.h"
#include "celtlib.h"

typedef struct window{
    struct window *parent_win;
    int x_pos,
        y_pos,
        rel_x_pos,
        rel_y_pos,
        width,
        height;
    char colour_bg,
         colour_fg;
} window;

typedef struct box_symbols{
    char *ch;
    char *ch_1;
    char *ch_2;
    char *ch_3;
    char *ch_4;
    char *ch_5;
    char *ch_6;
    char *ch_7;
} box_symbols;

void init_win(window *r_w);
void end_win();
void window_fill(window *win);
window create_window(window *par_win, char colour_bg, char colour_fg, int x_pos, int y_pos, int width, int height);
box_symbols set_box_symbols(char *ch, char *ch_1, char *ch_2, char *ch_3, char *ch_4, char *ch_5, char *ch_6, char *ch_7);
void window_line(window *win, char colour_bg, char colour_fg, int x0_pos, int y0_pos, int x1_pos, int y1_pos);
void window_rectangle(window *win, char colour_bg, char colour_fg, int x0_pos, int y0_pos, int x1_pos, int y1_pos);
void window_printxy(window *win, char colour_bg, char colour_fg, int ref_x_pos, int ref_y_pos, char *string);
void window_box(window *win, char colour_bg, box_symbols *bs, int ref_x0_pos, int ref_y0_pos, int ref_x1_pos, int ref_y1_pos);

#endif /* WINLIB_H */
