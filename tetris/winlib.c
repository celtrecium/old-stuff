#include "winlib.h"

void init_win(window *r_w) {
    clear();
    cl_noecho();
    cl_term_size(&r_w->width, &r_w->height);
    r_w->x_pos = 0;
    r_w->y_pos = 0;
}

void end_win() {
    set_position(1, 40);

    cl_echo();
}

void window_fill(window *win) {
    int x0_pos = win->x_pos;
    int y0_pos = win->y_pos;
    
    int x1_pos = win->width < win->parent_win->width 
        ? (x0_pos + win->width) : win->parent_win->width;

    int y1_pos = win->height < win->parent_win->height 
        ? (y0_pos + win->height) : win->parent_win->height;

    set_colour(win->colour_bg, win->colour_fg); 
    
    draw_rectangle(x0_pos, y0_pos, x1_pos, y1_pos, ' ');
}

window create_window(window *par_win, char colour_bg, char colour_fg, int x_pos, int y_pos, int width, int height) {
    int curr_x_pos =
        x_pos < par_win->x_pos + par_win->width 
        ? x_pos : par_win->width - par_win->x_pos;
    
    int curr_y_pos =
        y_pos < par_win->y_pos + par_win->height 
        ? y_pos : par_win->height - par_win->y_pos;
    
    int curr_width =
        (width + x_pos) < par_win->width 
        ? width : par_win->width - x_pos;
    
    int curr_height =
        (height + y_pos) < par_win->height 
        ? height : par_win->height - y_pos;
    
    window curr_win = { par_win, par_win->x_pos + curr_x_pos,
        par_win->y_pos + curr_y_pos, curr_x_pos, curr_y_pos, curr_width, curr_height, colour_bg, colour_fg };
    
    return(curr_win);
}

box_symbols set_box_symbols(char *ch, char *ch_1, char *ch_2, char *ch_3, char *ch_4, char *ch_5, char *ch_6, char *ch_7){
    box_symbols a = { ch, ch_1, ch_2, ch_3, ch_4, ch_5, ch_6, ch_7 };
    
    return(a);
};

void window_line(window *win, char colour_bg, char colour_fg, int x0_pos, int y0_pos, int x1_pos, int y1_pos){
    int x0 =
        x0_pos < win->width 
        ? x0_pos : win->width;
    
    int y0 =
        y0_pos < win->height 
        ? y0_pos : win->height;
    
    int x1 =
        x1_pos < win->width 
        ? x1_pos : win->width;
    
    int y1 =
        y1_pos < win->height 
        ? y1_pos : win->height - 1;
    
    set_colour(colour_bg, colour_fg);
    
    draw_line(win->x_pos + x0, win->y_pos + y0, win->x_pos + x1, win->y_pos + y1, ' ');
}

void window_rectangle(window *win, char colour_bg, char colour_fg, int x0_pos, int y0_pos, int x1_pos, int y1_pos){
    int x0 =
        x0_pos < win->width 
        ? x0_pos : win->width - 1;
    
    int y0 =
        y0_pos < win->height 
        ? y0_pos : win->height - 1;
    
    int x1 =
        x1_pos < win->width 
        ? x1_pos : win->width - 1;
    
    int y1 =
        y1_pos < win->height 
        ? y1_pos : win->height - 1;
    
    set_colour(colour_bg, colour_fg);
    
    draw_rectangle(win->x_pos + x0, win->y_pos + y0, win->x_pos + x1, win->y_pos + y1, ' ');
}

void window_printxy(window *win, char colour_bg, char colour_fg, int ref_x_pos, int ref_y_pos, char *string) {
    int x_pos =
        ref_x_pos < win->width 
        ? ref_x_pos : win->width;
    
    int y_pos =
        ref_y_pos < win->height 
        ? ref_y_pos : win->height - 1;
    
    int string_lenght = 0;
    
    int i = 0;
    int j = 0;

    for(; string[string_lenght]; ++string_lenght);

    set_colour(colour_bg, colour_fg);
    set_position(win->x_pos + x_pos, win->y_pos + y_pos);
    
    for(i = 0, j = 0; string[i]; ++i, ++j) {
        if(j > win->width - x_pos - 1) {
            y_pos +=
                (y_pos < win->height) 
                ? j = 0, x_pos = 0, 1 : 0;

            set_position(win->x_pos, win->y_pos + y_pos);
        }

        putc(*(string + i), stdout);
        
        if(string[i + 1] == '\n') {
            y_pos += (y_pos < win->height) ? ++i, x_pos = 0, 1 : 0;
            set_position(win->x_pos, win->y_pos + y_pos);
        }
    }
}

void window_box(window *win, char colour_bg, box_symbols *bs, int ref_x0_pos, int ref_y0_pos, int ref_x1_pos, int ref_y1_pos){
    int x0 =
        ref_x0_pos < win->width 
        ? ref_x0_pos : win->width;
    
    int y0 =
        ref_y0_pos < win->height 
        ? ref_y0_pos : win->height;
    
    int x1 =
        ref_x1_pos < win->width 
        ? ref_x1_pos : win->width;
    
    int y1 =
        ref_y1_pos < win->height 
        ? ref_y1_pos : win->height;

    int second_colour_fg = (colour_bg < 9) ? colour_bg + 8 : colour_bg - 8;
    
    if(colour_bg > 15) {
        second_colour_fg = 7;
    }

    draw_box(win->x_pos + x0, win->y_pos + y0, win->x_pos + x1, win->y_pos + y1, colour_bg, second_colour_fg, colour_bg, 0,
            bs->ch, bs->ch_1, bs->ch_2, bs->ch_3, bs->ch_4, bs->ch_5, bs->ch_6, bs->ch_7);
}
