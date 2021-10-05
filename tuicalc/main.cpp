#include<iostream>
#include<string>
#include<cstring>
#include<termios.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include "tuicalc.h"
#include "drawlib.h"
#include "settings.h"

std::string number_massive[ 20 ];
std::string action_massive[ 20 ];
std::string compose_num;
int action_n;
int number_n { 0 };
const int but_mas_1 { 5 };
const int but_mas_2 { 5 };
int win_size_h { 0 };
int win_size_w { 0 };

int calc_pos_x { 0 };
int calc_pos_y { 0 };

int calc_size_x0 { 1 + calc_pos_x };
int calc_size_y0 { 1 + calc_pos_y };
int calc_size_x1 { 21 + calc_pos_x };
int calc_size_y1 { 14 + calc_pos_y };
int calc_size_tmp { 0 };
int but_pos_x0 { 2 };
int but_pos_y0 { 5 };
int death { 0 };

static struct termios stored_settings;

namespace but { 
	int bnp_1 { 0 };
	int bnp_2 { 0 };
 }
namespace ind { 
	int equ { 0 };
	int equ_1 { 0 };
	int act { 0 };
	int pr { 0 };
	int sc { 0 };
 }

void win_size_test() { 
	struct winsize size;
	ioctl( 0, TIOCGWINSZ, (char *) &size );
	win_size_h = size.ws_row;
	win_size_w = size.ws_col;
 }

void set_keypress(void) { 
	struct termios new_settings;
	tcgetattr(0,&stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON & ~ECHO);
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&new_settings);
	return;
 }

void reset_keypress(void) { 
	tcsetattr(0,TCSANOW,&stored_settings);
	return;
 }

void select_button(int pos_1, int pos_2) { 
	std::string buttons [ but_mas_1 ] [ but_mas_2 ]= { 
		 { " Q ", " C ", " B ", " s ", " c " },
	         { " 1 ", " 2 ", " 3 ", " / ", " √ " },
	         { " 4 ", " 5 ", " 6 ", " * ", " ^ " },
	         { " 7 ", " 8 ", " 9 ", " - ", " ( " },
	         { " 0 ", " . ", " = ", " + ", " ) " }
	 };
        int but_pos_x2 { but_pos_x0 };
        int but_pos_y2 { but_pos_y0 };
        for(int i1 = 0; i1 <= but_mas_1-1; i1++) { 
                for(int i2 { 0 }; i2 <= but_mas_2-1; i2++) { 
                        drwlib::draw_button(but_pos_x2+calc_size_x0-1, but_pos_y2+calc_size_y0-1, buttons[ i1 ][ i2 ], button_color_bg, button_color_fg);
			if(pos_1 == i1 && pos_2 == i2) { 
				drwlib::draw_button(but_pos_x2+calc_size_x0-1, but_pos_y2+calc_size_y0-1, buttons[ i1 ][ i2 ], button_color_active_bg, button_color_active_fg);
			 }
                        but_pos_x2 += buttons[ i1 ][ i2 ].length() + 1;
                 }
                but_pos_x2 = but_pos_x0;
                but_pos_y2 = but_pos_y2 + 2;
         }
 }

void print_result(int pos_pr_x, int pos_pr_y, int indicator_1) { 
        std::string print_result_str;
        for(int count_1 { 0 }; count_1 <= number_n; count_1++) { 
                print_result_str += number_massive[count_1]+action_massive[count_1];
         }
        if(indicator_1 == 1) { 
                print_result_str += "=";
         }
	if(print_result_str.length() > calc_size_x1 - calc_pos_x - 2) { 
                print_result_str.resize(calc_size_x1 - calc_pos_x - 5);
                print_result_str += "...";
         }
        drwlib::draw_rect(pos_pr_x, pos_pr_y, 18, 1, num_screen_fg, num_screen_bg);
	drwlib::setcolor(40, 37);
	drwlib::printxy(pos_pr_x, pos_pr_y, print_result_str);
        drwlib::gotoxy(calc_size_x1, calc_size_y1);
        print_result_str = "";
 }

void print_numbers(int num_pos_x, int num_pos_y) { 
	drwlib::draw_rect(calc_size_x0+1, calc_size_y0+2, calc_size_x1-1, calc_size_y0+2, num_screen_fg, num_screen_bg);
	if(number_massive[number_n].length() > calc_size_x1 - calc_pos_x - 2) { 
		number_massive[number_n].pop_back();
	 }
	drwlib::setcolor(40, 37);
	drwlib::printxy(num_pos_x, num_pos_y, number_massive[number_n]);
	drwlib::gotoxy(calc_size_x1, calc_size_y1);
 }

void if_but_num_pos(int bnp_1, int bnp_2, std::string num_str) { 
	if(but::bnp_1 == bnp_1 && but::bnp_2 == bnp_2) { 
                number_massive[number_n] += num_str;
                print_numbers(calc_size_x0+1 ,calc_size_y0+2);
		ind::act = 1;
		ind::equ = 1;
		ind::equ_1 = 0;
         }
 }

void if_but_act_pos(int bnp_1, int bnp_2, std::string act_str_1, std::string act_str_2) { 
	if(ind::act == 1) { 
		if(but::bnp_1 == bnp_1 && but::bnp_2 == bnp_2) { 
			drwlib::draw_rect(calc_size_x0+1, calc_size_y0+1, calc_size_x1-1, calc_size_y0+2, num_screen_fg, num_screen_bg);
			drwlib::printxy(calc_size_x0+1 ,calc_size_y0+2, act_str_1);
        	        action_massive[action_n] = act_str_2;
        	        action_n++;
        	        number_n++;
        	        number_massive[number_n] = "";
			print_result(calc_size_x0+1, calc_size_y0+1, 0);
			ind::equ = 0;
			ind::equ_1 = 0;
			ind::pr = 1;
			ind::act = 0;
        	 }
	 }
 }

void equ_func(int bnp_1, int bnp_2, std::string ext_1, std::string ext_2, std::string ext_3) { 
	if(ind::equ == 1) { 
		if(but::bnp_1 == bnp_1 && but::bnp_2 == bnp_2) { 
                	std::string command_str;
                	for(int count_1 { 0 }; count_1 <= number_n; count_1++) { 
                	        command_str += number_massive[count_1]+action_massive[count_1];
                	 }
               		std::string com_bc_str = "echo '" + ext_1 + command_str + ext_2 + "' | bc" + ext_3;
                	char * com_bc = new char[com_bc_str.length()+1];
                	strcpy(com_bc, com_bc_str.c_str());
                	drwlib::draw_rect(calc_size_x0+1, calc_size_y0+1, calc_size_x1-1, calc_size_y0+2, num_screen_fg, num_screen_bg);
                	drwlib::gotoxy(calc_size_x0+1, calc_size_y0+2);
			system(com_bc);
			print_result(calc_size_x0+1, calc_size_y0+1, 1);
                	drwlib::gotoxy(calc_size_x1, calc_size_y1);
			ind::equ_1 = 1;
		 }
	 }
 }

void draw_logo() { 
	drwlib::draw_rect(calc_size_x0 + 21, calc_size_y0 + 4, calc_size_x1 - 1, calc_size_y1 - 1, logo_fg, logo_bg);
	drwlib::draw_pic(calc_size_x0 + 21, calc_size_y0 + 5, tcalc::pic_heigth, logo_bg, logo_fg, tcalc::pic);
}

void draw_logo_help() { 
	drwlib::draw_rect(calc_size_x0 + 21, calc_size_y0 + 4, calc_size_x1 - 1, calc_size_y1 - 1, logo_fg, logo_bg);
	drwlib::draw_pic(calc_size_x0 + 21, calc_size_y0 + 4, tcalc::help_heigth, logo_bg, logo_fg, tcalc::help);
}

void resize_window() { 
        win_size_test();
        if(calc_size_tmp != win_size_w) { 
                calc_size_x1 = win_size_w;
                if(win_size_h <= 12 + calc_size_y0 || win_size_w <= 21 + calc_size_x0) { 
			drwlib::clear();
                        death = 1;
                 }
                if(win_size_w >= 50  + calc_size_x0) { 
                        calc_size_x1 = 50 + calc_size_x0;
                 }
                drwlib::clear();
                drwlib::draw_rect(calc_size_x0, calc_size_y0, calc_size_x1, calc_size_y1, calc_color_fg, calc_color_bg);
                drwlib::draw_rect(calc_size_x0+1, calc_size_y0+1, calc_size_x1-1, calc_size_y0+2, num_screen_fg, num_screen_bg);
		if(win_size_w >= 50  + calc_size_x0) { 
                        draw_logo();
                 }
                if(ind::pr == 1) { 
                        print_result(calc_size_x0+1, calc_size_y0+1, 0);
                 }
                print_numbers(calc_size_x0+1 ,calc_size_y0+2);
		if(ind::equ_1 == 1) { 
			switch(ind::sc) { 
				case 0: equ_func(4, 2, "", "", ""); break;
				case 1: equ_func(4, 2, "", "", " -l"); break;
			 }
		 }
                calc_size_tmp = win_size_w;
         }
 }

void number_action() { 
	if_but_num_pos(1, 0, "1");
	if_but_num_pos(1, 1, "2");
	if_but_num_pos(1, 2, "3");
	if_but_num_pos(2, 0, "4");
	if_but_num_pos(2, 1, "5");
	if_but_num_pos(2, 2, "6");
	if_but_num_pos(3, 0, "7");
	if_but_num_pos(3, 1, "8");
	if_but_num_pos(3, 2, "9");
	if_but_num_pos(4, 0, "0");
	if_but_act_pos(1, 3, "÷", "/");
	if_but_act_pos(2, 3, "×", "*");
	if_but_act_pos(3, 3, "-", "-");
	if_but_act_pos(4, 3, "+", "+");
	if_but_act_pos(2, 4, "^", "^");
	if_but_num_pos(3, 4, "(");
	if_but_num_pos(4, 4, ")");
	if_but_num_pos(1, 4, "sqrt(");
	if_but_num_pos(0, 3, "s(");
	if_but_num_pos(0, 4, "c(");
	if_but_num_pos(4, 1, ".");
	if(ind::sc == 0) { 
	equ_func(4 ,2, "", "", "");
	 }
	else { 
	equ_func(4 ,2, "", "", " -l");
	 }
	if(but::bnp_1 == 0 && but::bnp_2 == 3) { 
	ind::sc = 1;
	 }
	if(but::bnp_1 == 0 && but::bnp_2 == 4) { 
        ind::sc = 1;
         }
	if(but::bnp_1 == 0 && but::bnp_2 == 2) { 
		if(number_massive[number_n] != "") { 
                        number_massive[number_n].pop_back();
                        print_numbers(calc_size_x0+1 ,calc_size_y0+2);
                 }
	 }
        if(but::bnp_1 == 0 && but::bnp_2 == 1) { 
		for(int count { 0 }; count <= number_n; count++) { 
			number_massive[count] = "";
		 }
		for(int count { 0 }; count <= action_n; count++) { 
                        action_massive[count] = "";
                 }
		number_n = 0;
                action_n = 0;
		drwlib::draw_rect(calc_size_x0+1, calc_size_y0+1, calc_size_x1-1, calc_size_y0+2, num_screen_fg, num_screen_bg);
		drwlib::gotoxy(calc_size_x1, calc_size_y1);
         }
	if(but::bnp_1 == 0 && but::bnp_2 == 0) { 
		drwlib::clear();
		death = 1;
		drwlib::cursor_mode(0);
	 }

 }

void num_act_key(int sel_pos_1, int sel_pos_2) { 
	resize_window();
        but::bnp_1 = sel_pos_1;
        but::bnp_2 = sel_pos_2;
        select_button(but::bnp_1, but::bnp_2);
        number_action();
 }

int main() { 
	drwlib::cursor_mode(1);
	resize_window();
	set_keypress();
	select_button(0, 0);
	while(true) { 
		if(death == 1) { 
			return 0;
		 }
		drwlib::gotoxy(calc_size_x1, calc_size_y1);
		switch(int symbol { getchar() }) { 
			case 67:
				resize_window();
				but::bnp_2 += 1;
				if(but::bnp_2 > but_mas_2-1) { 
			              	but::bnp_2 = 0;
				 }
		                select_button(but::bnp_1, but::bnp_2);
				break;
			case 68:
				resize_window();
				but::bnp_2 -= 1;
				if(but::bnp_2 < 0) { 
				        but::bnp_2 = but_mas_2-1;
				 }
				select_button(but::bnp_1, but::bnp_2);
				break;
			case 65:
				resize_window();
				but::bnp_1 -= 1;
				if(but::bnp_1 < 0) { 
					but::bnp_1 = but_mas_1-1;
				 }
				select_button(but::bnp_1, but::bnp_2);
				break;
			case 66:
				resize_window();
				but::bnp_1 += 1;
				if(but::bnp_1 > but_mas_1-1) { 
					but::bnp_1 = 0;
				 }
				select_button(but::bnp_1, but::bnp_2);
				break;
			case 10:
				resize_window();
				number_action();
				break;
			case 49: num_act_key(1, 0); break;
			case 50: num_act_key(1, 1); break;
			case 51: num_act_key(1, 2); break;
			case 52: num_act_key(2, 0); break;
			case 53: num_act_key(2, 1); break;
			case 54: num_act_key(2, 2); break;
			case 55: num_act_key(3, 0); break;
			case 56: num_act_key(3, 1); break;
			case 57: num_act_key(3, 2); break;
			case 48: num_act_key(4, 0); break;
			case 47: num_act_key(1, 3); break;
			case 42: num_act_key(2, 3); break;
			case 45: num_act_key(3, 3); break;
			case 43: num_act_key(4, 3); break;
			case 61: num_act_key(4, 2); break;
			case 99: num_act_key(0, 1); break;
			case 115: num_act_key(1, 4); break;
			case 94: num_act_key(2, 4); break;
			case 40: num_act_key(3, 4); break;
			case 41: num_act_key(4, 4); break;
			case 113: num_act_key(0, 0); break;
			case 100: num_act_key(0, 3); break;
			case 118: num_act_key(0, 4); break;
			case 127: num_act_key(0, 2); break;
			case 46: num_act_key(4, 1); break;
			case 104: if(win_size_w >= 50  + calc_size_x0) { draw_logo_help(); } break;
			case 98: if(win_size_w >= 50  + calc_size_x0) { draw_logo(); } break;
		 }
	 }
	return 0;
 }
