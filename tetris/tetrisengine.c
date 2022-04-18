#include "tetrisengine.h"

char *tetrs[7] = { 
	"__X___X___X___X_",
	"__X__XX__X______",
	"_X___XX___X_____",
	"_____XX__XX_____",
	"_XX___X___X_____",
	"_XX__X___X______",
	"__X__XX___X_____"
};


void set_tetris_movement(tetris_window *wi, char right, char left, char up, char down) {
    wi->tetr_movement.t_right = right;
    wi->tetr_movement.t_left = left;
    wi->tetr_movement.t_up = up;
    wi->tetr_movement.t_down = down;
}

void tetris_end(tetris_window *wt) {
	free(wt->field);
	free(wt->screen);
    free(wt->line);
	cl_echo();
	cl_cursor_mode(CL_SHOW);
	clear();
}

int rotate_tetromino(int x_pos, int y_pos, int deg) {
	switch(deg % 4) {
		case 0: 
			return(y_pos * 4 + x_pos); 
			break;

		case 1: 
			return(12 + y_pos - (x_pos * 4)); 
			break;

		case 2: 
			return(15 - (y_pos * 4) - x_pos); 
			break;

		case 3: 
			return(3 - y_pos + (x_pos * 4)); 
			break;
	}

	return(0);
}

int test_tetromino_pos(int tetr, int rot, int pos_x, int pos_y, tetris_window *tw) {
	int x = 0;
	int y = 0;

	for(x = 0; x < 4; ++x) {
		for(y = 0; y < 4; ++y) {
			int p = rotate_tetromino(x, y, rot);
			int f = (pos_y + y) * tw->field_width + (pos_x + x);

			if(pos_x + x >= 0 && pos_x + x < tw->field_width) {
				if(pos_y + y >= 0 && pos_y + y < tw->field_height) {
					if(tetrs[tetr][p] != '_' && tw->field[f] != 0) {
						
						return(0);
					}
				}
			}
		}
	}

	return(1);
}

void tetris_force_down(tetris_window *tw) {
    if(tw->force_down) {
		tw->speed_count = 0;
		++tw->tetr_count;

		if(tw->tetr_count % 50 == 0) {
			if(tw->speed >= 10) { 
				--tw->speed;
			}

			if(test_tetromino_pos(tw->curr_tetr, tw->curr_rot, tw->curr_x_pos, tw->curr_y_pos + 1, tw)) {
                ++tw->curr_y_pos;
            } 
            else {
				int temp_x = 0;
				int temp_y = 0;

				for(temp_x = 0; temp_x < 4; ++temp_x) {
					for(temp_y = 0; temp_y < 4; ++temp_y) {
						if(tetrs[tw->curr_tetr][rotate_tetromino(temp_x, temp_y, tw->curr_rot)] != '_') {
							tw->field[(tw->curr_y_pos + temp_y) * tw->field_width + (tw->curr_x_pos + temp_x)] = tw->curr_tetr + 1;
						}
					}
				}

				for(temp_y = 0; temp_y < 4; ++temp_y) {
					if(tw->curr_y_pos + temp_y < tw->field_height - 1) {
						int line_1 = 1;

						for(temp_x = 1; temp_x < tw->field_width - 1; ++temp_x) {
							line_1 &= (tw->field[(tw->curr_y_pos + temp_y) * tw->field_width + temp_x]) != 0;
						}

						if(line_1) {
							for(temp_x = 1; temp_x < tw->field_width - 1; ++temp_x) {
								tw->field[(tw->curr_y_pos + temp_y) * tw->field_width + temp_x] = 8;
							}

							++tw->line_arr_size;
							
							tw->line[tw->line_arr_size] = tw->curr_y_pos + temp_y;
						}
					}
				}

				tw->score += 25;

				if(tw->line_arr_size) {
					tw->score += (1 << tw->line_arr_size) * 100;
				}

				tw->curr_x_pos = tw->field_width / 2;
				tw->curr_y_pos = 0;
				tw->curr_rot = 0;
				tw->curr_tetr = rand() % 7;

				tw->gameover = !test_tetromino_pos(tw->curr_tetr, tw->curr_rot, tw->curr_x_pos, tw->curr_y_pos, tw);
			}
		}
	}
}

tetris_window create_tetris_window(int field_width, int field_height, int speed) {
    tetris_window tmp = {
        field_width,        /* field_width   */
        field_height,       /* field_height  */
        field_width,        /* screen_width  */
        field_height,       /* screen_height */

        4,                  /* square_width  */
        2,                  /* square_height */

        NULL,               /* *field        */
        NULL,               /* *screen       */
        0,                  /* gameover      */
        
        rand() % 7,         /* curr_tetr     */
        0,                  /* curr_rot      */
        field_width / 2,    /* curr_x_pos    */
        0,                  /* curr_y_pos    */
        speed,              /* speed         */
        0,                  /* speed_count   */
        0,                  /* force_down    */
        0,                  /* rot_hold      */
        0,                  /* tetr_count    */
        0,                  /* score         */
        0,                  /* line_arr_size */
        0,                  /* line_count    */
        NULL                /* *line         */
    };

    return(tmp);
}

void tetris_movement(char sym, int args, tetris_window *wt, ...) {
    tetris_window *ptr = wt; 

    for(; args > 0; ++ptr, --args) {
        ptr->rot_hold = 1;

        if(sym == ptr->tetr_movement.t_right) {
    		ptr->curr_x_pos += 
    		    (test_tetromino_pos(ptr->curr_tetr, ptr->curr_rot, ptr->curr_x_pos + 1, ptr->curr_y_pos, ptr)) 
    		    ? 1 : 0;
        } 
        else if(sym == ptr->tetr_movement.t_left) {
    		ptr->curr_x_pos -=
    			(test_tetromino_pos(ptr->curr_tetr, ptr->curr_rot, ptr->curr_x_pos - 1, ptr->curr_y_pos, ptr)) 
    			? 1 : 0;
        }
        else if(sym == ptr->tetr_movement.t_down) {
    		ptr->curr_y_pos +=
    			(test_tetromino_pos(ptr->curr_tetr, ptr->curr_rot, ptr->curr_x_pos, ptr->curr_y_pos + 1, ptr)) 
    			? 1 : 0;
        }
        else if(sym == ptr->tetr_movement.t_up) {
    		ptr->curr_rot +=
    			(ptr->rot_hold && test_tetromino_pos(ptr->curr_tetr, ptr->curr_rot + 1, ptr->curr_x_pos, ptr->curr_y_pos, ptr)) 
			    ? 1 : 0;
		    
		    ptr->rot_hold = 0;
        } 
        else if(sym == 'q') {
            for(; args > 0; ++ptr, --args) {
                tetris_end(ptr);
            }
            exit(0);
        }
    }
}

void tetris_display_buffer(tetris_window *wt) {
    int temp_x = 0;
    int temp_y = 0;
	char sym_array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 9, 8 };

	for(temp_x = 0; temp_x < wt->field_width; ++temp_x) {
		for(temp_y = 0; temp_y < wt->field_height; ++temp_y) {
			wt->screen[temp_y * wt->field_width + temp_x] = sym_array[wt->field[temp_y * wt->field_width + temp_x]];
		}
	}

	for(temp_x = 0; temp_x < 4; ++temp_x) {
		for(temp_y = 0; temp_y < 4; ++temp_y) {
			if(tetrs[wt->curr_tetr][rotate_tetromino(temp_x, temp_y, wt->curr_rot)] != '_') {
				wt->screen[(wt->curr_y_pos + temp_y) * wt->field_width + (wt->curr_x_pos + temp_x)] = wt->curr_tetr + 1;
			}
		}
	}
}

void tetris_count(tetris_window *wt) {
    ++wt->speed_count;
    wt->force_down = (wt->speed_count == wt->speed);
    /* usleep(10000); */
}

void tetris_burn_line(tetris_window *wt) {
	if(wt->line_arr_size) {
        int temp_x = 0;
        int temp_y = 0;
		int second_y = 0;
		
		for(temp_x = 1; temp_x < wt->field_width - 1; ++temp_x) {
			for(temp_y = 1; temp_y <= wt->line_arr_size; temp_y++) {
				for(second_y = wt->line[temp_y]; second_y > 0; --second_y) {
					wt->field[second_y * wt->field_width + temp_x] = wt->field[(second_y - 1) * wt->field_width + temp_x];
				}

				wt->field[temp_x] = 0;
			}
   		}
        
        wt->line_count += wt->line_arr_size;
   		wt->line_arr_size = 0;
	}
}
