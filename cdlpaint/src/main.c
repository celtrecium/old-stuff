#define _POSIX_C_SOURCE 199309L
#include <codl.h>
#include <time.h>

typedef struct paint_cursor {
    int  x_pos;
    int  y_pos;
    int  fg_color;
    int  bg_color;
    int  draw;
    char symbol[4];
    char cur_symbol[4];
} paint_cursor;

typedef enum FUNCS {
    P_SAVE,
    P_LOAD
} FUNCS;

void p_interrupt(unsigned int num) {
    struct timespec interrupt = {0, 0};
    struct timespec ret;
    interrupt.tv_nsec = num;
    nanosleep(&interrupt, &ret);
}

char *form_win(codl_window *par, int x_pos, int y_pos, int width) {
    codl_window *win2;
    char *inp = codl_malloc_check(width * 4 * (int)sizeof(char));
    unsigned int ch = 0;
    int count = 0;
    int len = 0;

    win2 = codl_create_window(par, codl_get_num_of_wins() + 1, x_pos, y_pos, width, 1);
    codl_memset(inp, width, 0, width);

    codl_set_colour(win2, 0, 256);
    
    codl_rectangle(win2, 0, 0, win2->width, 1, " ");
    codl_set_colour(win2, 2, 0);
    codl_replace_attributes(win2, count, 0, count, 0);

    codl_display();

    while(ch != CODL_KEY_RETURN) {
        if(ch) {
            codl_set_colour(win2, 0, 256);
            codl_set_cursor_position(win2, 0, 0);
            codl_rectangle(win2, 0, 0, win2->width, 1, " ");

            if(ch >= CODL_KEY_SPACE && ch <= CODL_KEY_TILDE && count < width - 1) {
                inp[count] = (char)ch;
                ++count;
            } else if(ch == CODL_KEY_BACKSPACE && count > 0) {
                --count;
                inp[count] = 0;
            } else if(ch == CODL_KEY_UNICODE && codl_strlen(codl_get_stored_key()) < (width)) {
                len = codl_strlen(codl_get_stored_key());
                codl_memcpy(&inp[count], 4 * (width - count), codl_get_stored_key(), len);
                count += len;
            }
            
            codl_write(win2, inp);
            codl_set_colour(win2, 2, 0);
            codl_write(win2, " ");

            codl_display();
        }

        p_interrupt(10000000);
        ch = codl_get_key();
    }

    codl_terminate_window(win2);
    
    return(inp);
}

long int get_value_win(char *question) {
    codl_window *win1;
    int q_len = codl_string_length(question);
    long int result;
    char *num = NULL;
    const int BG_COLOR = CODL_BRIGHT_BLUE;


    win1 = codl_create_window(NULL, codl_get_num_of_wins() + 1,
            (codl_get_term()->width - q_len + 4) / 2, (codl_get_term()->height - 5) / 2, q_len + 4, 5);
    
    codl_set_colour(win1, BG_COLOR, 0);
    codl_rectangle(win1, 0, 0, q_len + 4, 5, " ");
    codl_set_cursor_position(win1, 2, 1);
    codl_write(win1, question);

    num = form_win(win1, 2, 3, q_len);

    result = strtol(num, NULL, 10);
    free(num);
    codl_terminate_window(win1);
    
    return(result);
}

void get_symbol(char *question, char sym[4]) {
    codl_window *win1;
    int q_len = codl_string_length(question);
    char *num = NULL;
    const int BG_COLOR = CODL_BRIGHT_BLUE;

    win1 = codl_create_window(NULL, codl_get_num_of_wins() + 1,
            (codl_get_term()->width - q_len + 4) / 2, (codl_get_term()->height - 5) / 2, q_len + 4, 5);
    
    codl_set_colour(win1, BG_COLOR, 0);
    codl_rectangle(win1, 0, 0, q_len + 4, 5, " ");
    codl_set_cursor_position(win1, 2, 1);
    codl_write(win1, question);

    num = form_win(win1, 2, 3, q_len);

    if(codl_string_length(num) == 1) {
        codl_memset(sym, 4, 0, 4);
        codl_memcpy(sym, 4, num, codl_strlen(num));
    } else {
        codl_memset(sym, 4, 0, 4);
        sym[0] = num[0];
    }

    free(num);
    codl_terminate_window(win1);
}

void p_canvas_actions(char *question, codl_window *canvas, FUNCS mode) {
    codl_window *win1;
    int q_len = codl_string_length(question);
    char *num = NULL;
    const int BG_COLOR = CODL_BRIGHT_BLUE;
    FILE *s_file;

    win1 = codl_create_window(NULL, codl_get_num_of_wins() + 1,
            (codl_get_term()->width - q_len + 4) / 2, (codl_get_term()->height - 5) / 2, q_len + 4, 5);
    
    codl_set_colour(win1, BG_COLOR, 0);
    codl_rectangle(win1, 0, 0, q_len + 4, 5, " ");
    codl_set_cursor_position(win1, 2, 1);
    codl_write(win1, question);

    num = form_win(win1, 2, 3, q_len);

    switch(mode) {
        case P_SAVE:
            codl_save_buffer_to_file(canvas, num);
            break;
        case P_LOAD:
            codl_load_buffer_from_file(canvas, num, 0, 0);
            break;
    }

    free(num);
    codl_terminate_window(win1);
}

void help_scr(void) {
    const int HLP_WIDTH = 20;
    const int HLP_HEIGHT = 10;
    codl_window *win_help = codl_create_window(NULL, codl_get_num_of_wins() + 1, (codl_get_term()->width - HLP_WIDTH) / 2,
            (codl_get_term()->height - HLP_HEIGHT) / 2, HLP_WIDTH, HLP_HEIGHT);

    codl_set_colour(win_help, CODL_BRIGHT_WHITE, 0);
    codl_rectangle(win_help, 0, 0, win_help->width, win_help->height, " ");
    codl_write(win_help, "\n    \033[1;31mHelp screen\n");
    codl_write(win_help, " \033[1;2;31mC\033[30m — Change color\n");
    codl_write(win_help, " \033[1;2;31mI\033[30m — Change symbol\n");
    codl_write(win_help, " \033[1;2;31mS\033[30m — Save to file\n");
    codl_write(win_help, " \033[1;2;31mL\033[30m — Load from file\n");
    codl_write(win_help, " \033[1;2;31mN\033[30m — Clear canvas\n");
    
    codl_display();

    while(codl_get_key() != CODL_KEY_RETURN)
        p_interrupt(10000000);

    codl_terminate_window(win_help);

    return;
}

int main(void) {
    codl_window *win_frame;
    codl_window *win_canvas;
    codl_window *cur_space;
    int canvas_width  = 50;
    int canvas_height = 25;
    int canvas_pos_x  = 0;
    int canvas_pos_y  = 0;
    unsigned int key_pressed = 0;
    paint_cursor p_cur = {0, 0, CODL_DEFAULT_COLOUR, CODL_DEFAULT_COLOUR, 0, "*", " "};

    codl_initialize();
    canvas_width  = get_value_win("Enter canvas width");
    if(canvas_width > codl_get_term()->width - 6 || canvas_width <= 0)
        canvas_width = codl_get_term()->width - 6;

    canvas_height = get_value_win("Enter canvas height");
    if(canvas_height > codl_get_term()->height - 6 || canvas_height <= 0)
        canvas_height = codl_get_term()->height - 6;

    win_frame = codl_create_window(NULL, codl_get_num_of_wins() + 1, (codl_get_term()->width - canvas_width - 2) / 2,
            (codl_get_term()->height - canvas_height - 2) / 2, canvas_width + 2, canvas_height + 2);
    win_canvas = codl_create_window(win_frame, codl_get_num_of_wins() + 1, 1, 1, canvas_width, canvas_height);
    cur_space  = codl_create_window(win_canvas, codl_get_num_of_wins() + 1, 0, 0, win_canvas->width, win_canvas->height);

    codl_set_alpha(cur_space, CODL_ENABLE);
    codl_set_colour(win_canvas, CODL_BRIGHT_BLUE,  0);
    codl_set_colour(cur_space,  CODL_BRIGHT_WHITE, 0);
    codl_set_colour(win_frame, CODL_WHITE, 0);
    codl_rectangle(win_frame, 0, 0, win_frame->width, win_frame->height, " ");

    codl_set_cursor_position(cur_space, p_cur.x_pos, p_cur.y_pos);
    codl_write(cur_space, " ");

    codl_display();

    while(key_pressed != CODL_KEY_ESC) {
        switch(key_pressed) {
            case 0:
                break;
            case CODL_KEY_RIGHT:
                p_cur.x_pos += p_cur.x_pos + 1 < canvas_width ? 1 : 0;
                
                codl_window_clear(cur_space);

                codl_set_cursor_position(cur_space, p_cur.x_pos, p_cur.y_pos);
                codl_memcpy(p_cur.cur_symbol, 4, win_canvas->window_buffer[p_cur.x_pos][p_cur.y_pos], 4);
                if(!p_cur.cur_symbol[0]) p_cur.cur_symbol[0] = ' ';
                codl_write(cur_space, p_cur.cur_symbol);

                codl_display();
                break;

            case CODL_KEY_LEFT:
                p_cur.x_pos -= p_cur.x_pos - 1 >= 0 ? 1 : 0;

                codl_window_clear(cur_space);

                codl_set_cursor_position(cur_space, p_cur.x_pos, p_cur.y_pos);
                codl_memcpy(p_cur.cur_symbol, 4, win_canvas->window_buffer[p_cur.x_pos][p_cur.y_pos], 4);
                if(!p_cur.cur_symbol[0]) p_cur.cur_symbol[0] = ' ';
                codl_write(cur_space, p_cur.cur_symbol);



                codl_display();
                break;

            case CODL_KEY_UP:
                p_cur.y_pos -= p_cur.y_pos - 1 >= 0 ? 1 : 0;

                codl_window_clear(cur_space);
                
                codl_set_cursor_position(cur_space, p_cur.x_pos, p_cur.y_pos);
                codl_memcpy(p_cur.cur_symbol, 4, win_canvas->window_buffer[p_cur.x_pos][p_cur.y_pos], 4);
                if(!p_cur.cur_symbol[0]) p_cur.cur_symbol[0] = ' ';
                codl_write(cur_space, p_cur.cur_symbol);


                codl_display();
                break;

            case CODL_KEY_DOWN:
                p_cur.y_pos += p_cur.y_pos + 1 < canvas_height ? 1 : 0;
                
                codl_window_clear(cur_space);
 
                codl_set_cursor_position(cur_space, p_cur.x_pos, p_cur.y_pos);
                codl_memcpy(p_cur.cur_symbol, 4, win_canvas->window_buffer[p_cur.x_pos][p_cur.y_pos], 4);
                if(!p_cur.cur_symbol[0]) p_cur.cur_symbol[0] = ' ';
                codl_write(cur_space, p_cur.cur_symbol);

              
                codl_display();
                break;

            case CODL_KEY_SPACE:
                p_cur.draw = !p_cur.draw;
                codl_set_colour(cur_space, p_cur.draw ? CODL_GREEN : CODL_BRIGHT_WHITE, CODL_BLACK);

                codl_set_cursor_position(cur_space, p_cur.x_pos, p_cur.y_pos);
                codl_memcpy(p_cur.cur_symbol, 4, win_canvas->window_buffer[p_cur.x_pos][p_cur.y_pos], 4);
            
            if(!p_cur.cur_symbol[0]) p_cur.cur_symbol[0] = ' ';
            
            codl_write(cur_space, p_cur.cur_symbol);
                
                codl_display();
                break;

            case CODL_KEY_LC_H:
            case CODL_KEY_UC_H:
                help_scr();
                codl_display();

                break;
            case CODL_KEY_LC_C:
            case CODL_KEY_UC_C:
                p_cur.fg_color = get_value_win("Enter foreground color number");
                p_cur.bg_color = get_value_win("Enter background color number");
                
                codl_display();
                break;

            case CODL_KEY_LC_I:
            case CODL_KEY_UC_I:
                get_symbol("Enter symbol", p_cur.symbol);
                
                codl_display();
                break;

            case CODL_KEY_LC_S:
            case CODL_KEY_UC_S:
                p_canvas_actions("Enter file name for save", win_canvas, P_SAVE);

                codl_display();
                break;

            case CODL_KEY_LC_L:
            case CODL_KEY_UC_L:
                p_canvas_actions("Enter file name for load", win_canvas, P_LOAD);

                codl_display();
                break;
            
            case CODL_KEY_LC_N:
            case CODL_KEY_UC_N:
                codl_window_clear(win_canvas);

                codl_display();
                break;

            case CODL_KEY_RETURN:
                codl_set_colour(win_canvas, p_cur.bg_color, p_cur.fg_color);
                codl_set_cursor_position(win_canvas, p_cur.x_pos, p_cur.y_pos);
                codl_write(win_canvas, p_cur.symbol);
        
                codl_set_cursor_position(cur_space, p_cur.x_pos, p_cur.y_pos);
                codl_memcpy(p_cur.cur_symbol, 4, win_canvas->window_buffer[p_cur.x_pos][p_cur.y_pos], 4);
                
                if(!p_cur.cur_symbol[0]) p_cur.cur_symbol[0] = ' ';
                
                codl_write(cur_space, p_cur.cur_symbol);
        }
        
        if(p_cur.draw) {
            codl_set_colour(win_canvas, p_cur.bg_color, p_cur.fg_color);
            codl_set_cursor_position(win_canvas, p_cur.x_pos, p_cur.y_pos);
            codl_write(win_canvas, p_cur.symbol);
        
            codl_set_cursor_position(cur_space, p_cur.x_pos, p_cur.y_pos);
            codl_memcpy(p_cur.cur_symbol, 4, win_canvas->window_buffer[p_cur.x_pos][p_cur.y_pos], 4);
            
            if(!p_cur.cur_symbol[0]) p_cur.cur_symbol[0] = ' ';
            
            codl_write(cur_space, p_cur.cur_symbol);
        }

                codl_display();
 
        key_pressed = codl_get_key();
        
        p_interrupt(10000000);
    }

    codl_display();

    codl_end();
    codl_clear();

    return(0);
}
