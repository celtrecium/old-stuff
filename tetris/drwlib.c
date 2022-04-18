#include "drwlib.h"

void dl_puts(const char *string) {
    int size = 1;
    
    for(; string[size] != '\0'; ++size);
    
    syscall(SYS_write, 1, string, size + 1);
}

void set_colour(unsigned int bg, unsigned int fg){
    printf("\033[38;05;%d;48;05;%dm", fg, bg);
}

void set_position(int x_pos, int y_pos){
    printf("\033[%d;%dH", y_pos, x_pos);
}

void clear(){
    dl_puts("\033[49;39m\033[H\033[2J\n");
}

void printxy(int x_pos, int y_pos, char symbol){
    set_position(x_pos, y_pos);

    syscall(SYS_write, 1, symbol, sizeof(char));
}

void draw_rectangle(int x0_pos, int y0_pos, int x1_pos, int y1_pos, char symbol){
    int width = x1_pos - x0_pos;
    int height = y1_pos - y0_pos;
    
    int count_1 = 0;
    int count_2 = 0;

    set_position(x0_pos, y0_pos);

    for(count_1 = 0; count_1 < height; ++count_1){
        for(count_2 = 0; count_2 < width; ++count_2){
            putc(symbol, stdout);
        }

        printf("\n\033[%dG", x0_pos);
    }
}

void draw_line(int x1, int y1, int x2, int y2, char symbol) {
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    
    int error = deltaX - deltaY;

    while(x1 != x2 || y1 != y2) {
        printxy(x1, y1, symbol);
        
        const int error2 = error * 2;
        
        if(error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }

        if(error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }                    
}

void draw_box(int x0_pos, int y0_pos, int x1_pos, int y1_pos, int bg0, int fg0, int bg1, int fg1,
        char *ch, char *ch_1, char *ch_2, char *ch_3, char *ch_4, char *ch_5, char *ch_6, char *ch_7) {
    
    int width = x1_pos > x0_pos ? x1_pos - x0_pos : x0_pos - x1_pos;
    int height = y1_pos > y0_pos ? y1_pos - y0_pos : y0_pos - y1_pos;
    int i = 0;

    set_position(x0_pos, y0_pos);
    
    printf("\033[38;05;%d;48;05;%dm%s", fg0, bg0, ch_4);
    
    for(i = 0; i < width - 2; ++i){
        fputs(ch_2, stdout);
    };

    printf("\033[38;05;%d;48;05;%dm%s\n", fg1, bg1, ch_5);
    
    for(i = 0; i < height - 2; ++i){
        printf("\033[38;05;%d;48;05;%dm\033[%dG%s\033[%dG\033[38;05;%d;48;05;%dm%s\n",
                fg0, bg0, x0_pos, ch, x0_pos + width - 1, fg1, bg1, ch_1);
    };
    
    printf("\033[38;05;%d;48;05;%dm\033[%dG%s\033[38;05;%d;48;05;%dm",
            fg0, bg0, x0_pos, ch_6, fg1, bg1);
    
    for(i = 0; i < width - 2; ++i){
        fputs(ch_3, stdout);
    };
    
    fputs(ch_7, stdout);

};
