#include<iostream>
#include<string>
#include<cstring>
#include<termios.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include "settings.h"

std::string number_massive[20];
std::string action_massive[20];
int action_n;
int number_n{0};
const int but_mas_1{5};
const int but_mas_2{5};
int selected_pos_1{0};
int selected_pos_2{0};
int indicator_2{0};
int indicator_3{0};
int indicator_equ{0};
int indicator_sc{0};
int win_size_h{0};
int win_size_w{0};

int calc_pos_x{0};
int calc_pos_y{0};

int calc_size_x0{1 + calc_pos_x};
int calc_size_y0{1 + calc_pos_y};
int calc_size_x1{21 + calc_pos_x};
int calc_size_y1{14 + calc_pos_y};
int calc_size_tmp{0};
int but_pos_x0{2};
int but_pos_y0{5};
int death{0};

static struct termios stored_settings;

void win_size_test(){
	struct winsize size;
	ioctl( 0, TIOCGWINSZ, (char *) &size );
	win_size_h = size.ws_row;
	win_size_w = size.ws_col;
}

void set_keypress(void){
	struct termios new_settings;
	tcgetattr(0,&stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON & ~ECHO);
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&new_settings);
	return;
}

void reset_keypress(void){
	tcsetattr(0,TCSANOW,&stored_settings);
	return;
}

void clear(){std::cout << "\033[49;37m" << std::flush << "\033[H\033[2J" << std::flush;}

void setcolor(int bg, int fg){std::cout << "\033[" << bg << ";" << fg << "m" << std::flush;}

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

void draw_button(int button_y, int button_x, std::string text, int bg, int fg){
	std::cout << "\033[" << button_x << ";" << button_y << "H";
	std::cout << "\033[" << bg << ";" << fg << "m " << text << " \033[0m" << std::flush;
}

void gotoxy(int x, int y){
	std::cout << "\033[" << y << ";" << x << "H" << std::flush;
}

void select_button(int pos_1, int pos_2){
	std::string buttons[but_mas_1][but_mas_2]={
		{"Q", "C", "B", "s", "c"},
	        {"1", "2", "3", "÷", "√"},
	        {"4", "5", "6", "×", "^"},
	        {"7", "8", "9", "-", "("},
	        {"0", ".", "=", "+", ")"}
	};
        int but_pos_x2{but_pos_x0};
        int but_pos_y2{but_pos_y0};
        for(int i1 = 0; i1 <= but_mas_1-1; i1++){
                for(int i2{0}; i2 <= but_mas_2-1; i2++){
                        draw_button(but_pos_x2+calc_size_x0-1, but_pos_y2+calc_size_y0-1, buttons[i1][i2], button_color_bg, button_color_fg);
			if(pos_1 == i1 && pos_2 == i2){
				draw_button(but_pos_x2+calc_size_x0-1, but_pos_y2+calc_size_y0-1, buttons[i1][i2], button_color_active_bg, button_color_active_fg);
			}
                        but_pos_x2 = but_pos_x2 + 4;
                }
                but_pos_x2 = but_pos_x0;
                but_pos_y2 = but_pos_y2 + 2;
        }
	std::cout << std::endl;
}

std::string compose_num;

void print_result_u(int pos_pr_x, int pos_pr_y, int indicator_1){
        std::string print_result_str;
        for(int count_1{0}; count_1 <= number_n; count_1++){
                print_result_str += number_massive[count_1]+action_massive[count_1];
        }
        if(indicator_1 == 1){
                print_result_str += "=";
        }
	if(print_result_str.length() > calc_size_x1 - calc_pos_x - 2){
                print_result_str.resize(calc_size_x1 - calc_pos_x - 5);
                print_result_str += "...";
        }
        draw_rect(pos_pr_x, pos_pr_y, 18, 1, num_screen_fg, num_screen_bg);
        gotoxy(pos_pr_x, pos_pr_y);
        std::cout << print_result_str << std::flush;
        gotoxy(calc_size_x1, calc_size_y1);
        print_result_str = "";
}

void print_numbers(int num_pos_x, int num_pos_y){
	draw_rect(calc_size_x0+1, calc_size_y0+2, calc_size_x1-1, calc_size_y0+2, num_screen_fg, num_screen_bg);
	gotoxy(num_pos_x, num_pos_y);
	if(number_massive[number_n].length() > calc_size_x1 - calc_pos_x - 2){
		number_massive[number_n].pop_back();
	}
	std::cout << number_massive[number_n];
	gotoxy(calc_size_x1, calc_size_y1);
}

void if_but_num_pos(int but_num_pos_1, int but_num_pos_2, int bnp_1, int bnp_2, std::string num_str, int pn_x, int pn_y){
	if(but_num_pos_1 == bnp_1 && but_num_pos_2 == bnp_2){
                number_massive[number_n] += num_str;
                print_numbers(pn_x, pn_y);
		indicator_2 = 1;
		indicator_equ = 0;
        }
}

void if_but_act_pos(int but_num_pos_1, int but_num_pos_2, int bnp_1, int bnp_2, std::string act_str_1, std::string act_str_2, int pn_x, int pn_y){
	if(but_num_pos_1 == bnp_1 && but_num_pos_2 == bnp_2){
		draw_rect(calc_size_x0+1, calc_size_y0+1, calc_size_x1-1, calc_size_y0+2, num_screen_fg, num_screen_bg);
		gotoxy(pn_x, pn_y);
		std::cout << act_str_1 << std::flush;
                action_massive[action_n] = act_str_2;
                action_n++;
                number_n++;
                number_massive[number_n] = "";
		print_result_u(calc_size_x0+1, calc_size_y0+1, 0);
		indicator_2 = 0;
		indicator_3 = 1;
		indicator_equ = 0;
        }
}

void equ_func(int but_num_pos_1, int but_num_pos_2, int bnp_1, int bnp_2, std::string ext_1, std::string ext_2, std::string ext_3){
	if(indicator_2 == 1){
		if(but_num_pos_1 == bnp_1 && but_num_pos_2 == bnp_2){
                	std::string command_str;
                	for(int count_1{0}; count_1 <= number_n; count_1++){
                	        command_str += number_massive[count_1]+action_massive[count_1];
                	}
               		std::string com_bc_str = "echo '" + ext_1 + command_str + ext_2 + "' | bc" + ext_3;
                	char * com_bc = new char[com_bc_str.length()+1];
                	strcpy(com_bc, com_bc_str.c_str());
                	draw_rect(calc_size_x0+1, calc_size_y0+1, calc_size_x1-1, calc_size_y0+2, num_screen_fg, num_screen_bg);
                	gotoxy(calc_size_x0+1, calc_size_y0+2);
			system(com_bc);
			print_result_u(calc_size_x0+1, calc_size_y0+1, 1);
                	gotoxy(calc_size_x1, calc_size_y1);
			indicator_equ = 1;
			indicator_sc = 0;
		}
	}
}

void draw_logo(){
	draw_rect(calc_size_x0+21, calc_size_y0+4, calc_size_x1-1, calc_size_y1-1, logo_fg, logo_bg);
	gotoxy(calc_size_x0+21, calc_size_y0+5);
	setcolor(logo_bg, logo_fg);
	std::cout << "           TUICalc";
	gotoxy(calc_size_x0+21, calc_size_y0+6);
	std::cout << "            v0.1";
	gotoxy(calc_size_x0+21, calc_size_y0+7);
        std::cout << "          h - help";
	gotoxy(calc_size_x0+21, calc_size_y0+9);
        std::cout << "         ▄▀▄     ▄▀▄";
	gotoxy(calc_size_x0+21, calc_size_y0+10);
        std::cout << "        ▄█░░▀▀▀▀▀░░█▄";
	gotoxy(calc_size_x0+21, calc_size_y0+11);
        std::cout << "    ▄▄  █░░░░░░░░░░░█  ▄▄";
	gotoxy(calc_size_x0+21, calc_size_y0+12);
        std::cout << "   █▄▄█ █░░▀░░┬░░▀░░█ █▄▄█";
}

void draw_logo_help(){
        draw_rect(calc_size_x0+21, calc_size_y0+4, calc_size_x1-1, calc_size_y1-1, logo_fg, logo_bg);
        gotoxy(calc_size_x0+21, calc_size_y0+4);
        setcolor(logo_bg, logo_fg);
        std::cout << "        Help screen";
        gotoxy(calc_size_x0+21, calc_size_y0+5);
        std::cout << " c - clear";
        gotoxy(calc_size_x0+21, calc_size_y0+6);
        std::cout << " s - sqrt";
        gotoxy(calc_size_x0+21, calc_size_y0+7);
        std::cout << " d - sin";
        gotoxy(calc_size_x0+21, calc_size_y0+8);
        std::cout << " v - cos";
        gotoxy(calc_size_x0+21, calc_size_y0+9);
        std::cout << " q - quit";
        gotoxy(calc_size_x0+21, calc_size_y0+12);
        std::cout << " b - back to main screen";
}

void resize_window(){
        win_size_test();
        if(calc_size_tmp != win_size_w){
                calc_size_x1 = win_size_w;
                if(win_size_h <= 12 + calc_size_y0 || win_size_w <= 21 + calc_size_x0){
			clear();
                        std::cout << "smol pepis" << std::endl;
                        death = 1;
                }
                if(win_size_w >= 50  + calc_size_x0){
                        calc_size_x1 = 50 + calc_size_x0;
                }
                clear();
                draw_rect(calc_size_x0, calc_size_y0, calc_size_x1, calc_size_y1, calc_color_fg, calc_color_bg);
                draw_rect(calc_size_x0+1, calc_size_y0+1, calc_size_x1-1, calc_size_y0+2, num_screen_fg, num_screen_bg);
		if(win_size_w >= 50  + calc_size_x0){
                        draw_logo();
                }
                if(indicator_3 == 1){
                        print_result_u(calc_size_x0+1, calc_size_y0+1, 0);
                }
                print_numbers(calc_size_x0+1 ,calc_size_y0+2);
		if(indicator_equ == 1){
			if(indicator_sc == 0){
        			equ_func(4, 2, 4 ,2, "", "", "");
        		}
        		else{
        			equ_func(4, 2, 4 ,2, "", "", " -l");
        		}
		}
                calc_size_tmp = win_size_w;
        }
}

void number_action(int but_num_pos_1, int but_num_pos_2){
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 1, 0, "1", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 1, 1, "2", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 1, 2, "3", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 2, 0, "4", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 2, 1, "5", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 2, 2, "6", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 3, 0, "7", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 3, 1, "8", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 3, 2, "9", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 4, 0, "0", calc_size_x0+1 ,calc_size_y0+2);
	if_but_act_pos(but_num_pos_1, but_num_pos_2, 1, 3, "÷", "/", calc_size_x0+1 ,calc_size_y0+2);
	if_but_act_pos(but_num_pos_1, but_num_pos_2, 2, 3, "×", "*", calc_size_x0+1 ,calc_size_y0+2);
	if_but_act_pos(but_num_pos_1, but_num_pos_2, 3, 3, "-", "-", calc_size_x0+1 ,calc_size_y0+2);
	if_but_act_pos(but_num_pos_1, but_num_pos_2, 4, 3, "+", "+", calc_size_x0+1 ,calc_size_y0+2);
	if_but_act_pos(but_num_pos_1, but_num_pos_2, 2, 4, "^", "^", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 3, 4, "(", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 4, 4, ")", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 1, 4, "sqrt(", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 0, 3, "s(", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 0, 4, "c(", calc_size_x0+1 ,calc_size_y0+2);
	if_but_num_pos(but_num_pos_1, but_num_pos_2, 4, 1, ".", calc_size_x0+1 ,calc_size_y0+2);
	if(indicator_sc == 0){
	equ_func(but_num_pos_1, but_num_pos_2, 4 ,2, "", "", "");
	}
	else{
	equ_func(but_num_pos_1, but_num_pos_2, 4 ,2, "", "", " -l");
	}
	if(but_num_pos_1 == 0 && but_num_pos_2 == 3){
	indicator_sc = 1;
	}
	if(but_num_pos_1 == 0 && but_num_pos_2 == 4){
        indicator_sc = 1;
        }
	if(but_num_pos_1 == 0 && but_num_pos_2 == 2){
		if(number_massive[number_n] != ""){
                        number_massive[number_n].pop_back();
                        print_numbers(calc_size_x0+1 ,calc_size_y0+2);
                }
	}
        if(but_num_pos_1 == 0 && but_num_pos_2 == 1){
		for(int count{0}; count <= number_n; count++){
			number_massive[count] = "";
		}
		for(int count{0}; count <= action_n; count++){
                        action_massive[count] = "";
                }
		number_n = 0;
                action_n = 0;
		draw_rect(calc_size_x0+1, calc_size_y0+1, calc_size_x1-1, calc_size_y0+2, num_screen_fg, num_screen_bg);
		gotoxy(calc_size_x1, calc_size_y1);
        }
	if(but_num_pos_1 == 0 && but_num_pos_2 == 0){
		clear();
		death = 1;
	}

}

void num_act_key(int sel_pos_1, int sel_pos_2){
	resize_window();
        selected_pos_1 = sel_pos_1;
        selected_pos_2 = sel_pos_2;
        select_button(selected_pos_1, selected_pos_2);
        number_action(sel_pos_1, sel_pos_2);
}

int main(){
	resize_window();
	set_keypress();
	select_button(0, 0);
	while(true){
		if(death == 1){
			return 0;
		}
		gotoxy(calc_size_x1, calc_size_y1);
		switch(int symbol{getchar()}){
			case 67:
				resize_window();
				selected_pos_2 += 1;
				if(selected_pos_2 > but_mas_2-1){
			              	selected_pos_2 = 0;
				}
		                select_button(selected_pos_1, selected_pos_2);
				break;
			case 68:
				resize_window();
				selected_pos_2 -= 1;
				if(selected_pos_2 < 0){
				        selected_pos_2 = but_mas_2-1;
				}
				select_button(selected_pos_1, selected_pos_2);
				break;
			case 65:
				resize_window();
				selected_pos_1 -= 1;
				if(selected_pos_1 < 0){
					selected_pos_1 = but_mas_1-1;
				}
				select_button(selected_pos_1, selected_pos_2);
				break;
			case 66:
				resize_window();
				selected_pos_1 += 1;
				if(selected_pos_1 > but_mas_1-1){
					selected_pos_1 = 0;
				}
				select_button(selected_pos_1, selected_pos_2);
				break;
			case 10:
				resize_window();
				number_action(selected_pos_1, selected_pos_2);
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
			case 104: draw_logo_help(); break;
			case 98: draw_logo(); break;
		}
	}
	std::cout << std::endl;
	return 0;
}
