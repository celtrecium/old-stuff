#ifndef TETRISBASE_H
#define TETRISBASE_H

#include <stdio.h>
#include <stdlib.h>
#include "celtlib.h"
#include "drwlib.h"

typedef struct tetris_move {
    char t_right,
         t_left,
         t_up,
         t_down;
} tetris_move;

typedef struct tetris_window {
    const unsigned int field_width;
    const unsigned int field_height;
    unsigned int screen_width;
    unsigned int screen_height;

    int square_width;
    int square_height;

    char *field;
    char *screen;
    int gameover;

    int curr_tetr;
	int curr_rot;
	int curr_x_pos;
	int curr_y_pos;
	int speed;
	int speed_count;
	int force_down;
	int rot_hold;
	int tetr_count;
	int score;
	int line_arr_size;
    int line_count;
	int *line;
    tetris_move tetr_movement;
} tetris_window;

void set_tetris_movement(tetris_window *wi, char right, char left, char up, char down);
void tetris_end(tetris_window *wt);
int rotate_tetromino(int x_pos, int y_pos, int deg);
void tetris_force_down(tetris_window *tw);
tetris_window create_tetris_window(int field_width, int field_height, int speed);
void tetris_movement(char sym, int args, tetris_window *wt, ...);
void tetris_display_buffer(tetris_window *wt);
void tetris_count(tetris_window *wt);
void tetris_burn_line(tetris_window *wt);

#endif /* TETRISBASE_H */
