/* Library for simple drawing */

/* Clear screen */
void clear(){std::cout << "\033[49;37m" << std::flush << "\033[H\033[2J" << std::flush;}

void setcolor(int bg, int fg){std::cout << "\033[" << bg << ";" << fg << "m" << std::flush;}

/* Set cursor mode (1 = hide cursor, 2 = show cursor) */

void cursor_mode(int mode){
        switch(mode){
                case 0: std::cout << "\033[?25h" << std::flush; break;
                case 1: std::cout << "\033[?25l" << std::flush; break;
        }
}

/* Draw rectangle */

void draw_rect(int rect_h_0, int rect_w_0, int rect_h_1, int rect_w_1, int rect_colour_fg, int rect_colour_bg){
        int delta_r_h{rect_h_1 - rect_h_0};
        int delta_r_w{rect_w_1 - rect_w_0};
        std::string buff;
        for(int count_1{0}; count_1 <= delta_r_h; count_1++){
                int cprw{count_1 + rect_h_0};
                for(int count_2{0}; count_2 <= delta_r_w; count_2++){
                        int cprh{count_2 + rect_w_0};
                        buff += "\033[" + std::to_string(cprh) + ";" + std::to_string(cprw) + "H" + "\033[" + std::to_string(rect_colour_bg) + ";" + std::to_string(rect_colour_fg) + "m ";
                }
        }
        std::cout << buff << std::flush;
}

/* Go to coordinates X, Y */

void gotoxy(int x, int y){
        std::cout << "\033[" << y << ";" << x << "H" << std::flush;
}

/* Draw simpe button with text */

void draw_button(int button_y, int button_x, std::string text, int bg, int fg){
        std::cout << "\033[" << button_x << ";" << button_y << "H";
        std::cout << "\033[" << bg << ";" << fg << "m " << text << " \033[0m" << std::flush;
}

/* Print text to X, Y */

void printxy(int x, int y, std::string str){
	gotoxy(x, y);
	std::cout << str;
}
