/* Library for simple drawing */
#ifndef DRAWLIB_H
#define DRAWLIB_H

namespace drwlib{
	/* Clear screen */
	void clear(){std::cout << "\033[49;37m" << std::flush << "\033[H\033[2J" << std::flush;}
	
	void setcolor(int bg, int fg){std::cout << "\033[" << bg << ";" << fg << "m" << std::flush;}
	
	/* Set cursor mode (0 = show cursor, 1 = hide cursor) */
	
	void cursor_mode(int mode){
	        switch(mode){
	                case 0: std::cout << "\033[?25h" << std::flush; break;
	                case 1: std::cout << "\033[?25l" << std::flush; break;
	        }
	}
	
	/* Draw rectangle */
	
	void draw_rect(int rect_w_0, int rect_h_0, int rect_w_1, int rect_h_1, int rect_colour_fg, int rect_colour_bg){
		int delta_r_h{rect_h_1 - rect_h_0};
	        int delta_r_w{rect_w_1 - rect_w_0};
		std::string spaces;
		std::string spaces_1;
		for(int count{0}; count <= delta_r_w; ++count){
			spaces += " ";
		}
		for(int count{0}; count <= delta_r_h; ++count){
			int cprh{rect_h_0 + count};
			spaces_1 += "\033[" + std::to_string(cprh) + ";" + std::to_string(rect_w_0) + "H" + spaces;
		}
		std::string buff{"\033[" + std::to_string(rect_colour_bg) + ";" + std::to_string(rect_colour_fg) + "m" + spaces_1};
		std::cout << buff << std::flush;
	}

	/* Go to cordinates X, Y */

	void gotoxy(int x, int y){
	        std::cout << "\033[" << y << ";" << x << "H" << std::flush;
	}

	/* Draw simpe button with text */
	
	void draw_button(int button_y, int button_x, std::string text, int bg, int fg){
	        std::cout << "\033[" << button_x << ";" << button_y << "H";
	        std::cout << "\033[" << bg << ";" << fg << "m" << text << "\033[0m" << std::flush;
	}
	
	/* Print text to X, Y */
	
	void printxy(int x, int y, std::string str){
		gotoxy(x, y);
		std::cout << str;
	}
	void draw_pic(int pos_x, int pos_y, int heigth, int bg, int fg, std::string str[]){
		drwlib::setcolor(bg, fg);
		for(int count { 0 }; count <= heigth-1; ++count) { 
			drwlib::printxy(pos_x, pos_y + count, str[count]);
	 	}
	}
}

#endif /* DRAWLIB_H */
