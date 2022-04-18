#include<iostream>
#include<string>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<termios.h>

std::string number_massive[4];
std::string action_massive[4];
int action_n;
int number_n = 0;
static struct termios stored_settings;

//Начало не моих кусков кода
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
//Конец не моих кусков кода

void draw_button(int button_y, int button_x, std::string text, int bg, int fg){
	std::cout << "\033[" << button_x << ";" << button_y << "H";
	std::cout << "\033[" << bg << ";" << fg << "m " << text << " \033[0m" << std::flush;
}

void select_button(int pos_1, int pos_2){
	std::string buttons[4][4]={
                {"1", "2", "3", "÷"},
                {"4", "5", "6", "×"},
                {"7", "8", "9", "-"},
                {"0", "C", "=", "+"}
        };
        int but_pos_x0 = 2;
        int but_pos_y0 = 4;
        int but_pos_x2 = but_pos_x0;
        int but_pos_y2 = but_pos_y0;
        for(int i1 = 0; i1 <= 3; i1++){
                for(int i2 = 0; i2 <= 3; i2++){
                        draw_button(but_pos_x2, but_pos_y2, buttons[i1][i2], 47, 30);
			if(pos_1 == i1 && pos_2 == i2){
				draw_button(but_pos_x2, but_pos_y2, buttons[i1][i2], 44, 30);
			}
                        but_pos_x2 = but_pos_x2 + 4;
                }
                but_pos_x2 = but_pos_x0;
                but_pos_y2 = but_pos_y2 + 2;
        }
	std::cout << std::endl;
}

std::string compose_num;

void print_numbers(int num_pos_x, int num_pos_y){
	std::cout << "\033[" << num_pos_x << ";" << num_pos_y << "H";
	std::cout << "\033[40;37m" << number_massive[number_n];
	std::cout << "\033[11;0H" << std::flush;
}


void if_but_num_pos(int but_)

void number_action(int but_num_pos_1, int but_num_pos_2){
	if(but_num_pos_1 == 0 && but_num_pos_2 == 0){
		number_massive[number_n] += "1";
		print_numbers(2, 2);
	}
        if(but_num_pos_1 == 0 && but_num_pos_2 == 1){
                number_massive[number_n] += "2";
		print_numbers(2, 2);
        }
        if(but_num_pos_1 == 0 && but_num_pos_2 == 2){
                number_massive[number_n] += "3";
		print_numbers(2, 2);
        }
        if(but_num_pos_1 == 1 && but_num_pos_2 == 0){
                number_massive[number_n] += "4";
		print_numbers(2, 2);
        }
        if(but_num_pos_1 == 1 && but_num_pos_2 == 1){
                number_massive[number_n] += "5";
		print_numbers(2, 2);
        }
        if(but_num_pos_1 == 1 && but_num_pos_2 == 2){
                number_massive[number_n] += "6";
		print_numbers(2, 2);
        }
        if(but_num_pos_1 == 2 && but_num_pos_2 == 0){
                number_massive[number_n] += "7";
		print_numbers(2, 2);
        }
        if(but_num_pos_1 == 2 && but_num_pos_2 == 1){
                number_massive[number_n] += "8";
		print_numbers(2, 2);
        }
        if(but_num_pos_1 == 2 && but_num_pos_2 == 2){
                number_massive[number_n] += "9";
		print_numbers(2, 2);
        }
        if(but_num_pos_1 == 3 && but_num_pos_2 == 0){
                number_massive[number_n] += "0";
		print_numbers(2, 2);
        }
        if(but_num_pos_1 == 3 && but_num_pos_2 == 3){
		std::cout << "\033[40;37m\033[2;2H" << "+              " << "\033[11;1H" << std::flush;
                action_massive[action_n] = "+";
		action_n++;
		number_n++;
		number_massive[number_n] = "";
        }
	if(but_num_pos_1 == 2 && but_num_pos_2 == 3){
                std::cout << "\033[40;37m\033[2;2H" << "-              " << "\033[11;1H" << std::flush;
                action_massive[action_n] = "-";
                action_n++;
                number_n++;
		number_massive[number_n] = "";
        }
	if(but_num_pos_1 == 1 && but_num_pos_2 == 3){
                std::cout << "\033[40;37m\033[2;2H" << "×              " << "\033[11;1H" << std::flush;
                action_massive[action_n] = "*";
                action_n++;
                number_n++;
		number_massive[number_n] = "";
        }
	 if(but_num_pos_1 == 0 && but_num_pos_2 == 3){
                std::cout << "\033[40;37m\033[2;2H" << "÷              " << "\033[11;1H" << std::flush;
                action_massive[action_n] = "/";
                action_n++;
                number_n++;
		number_massive[number_n] = "";
        }
	if(but_num_pos_1 == 3 && but_num_pos_2 == 1){
		number_n = 0;
		action_n = 0;
		number_massive[number_n] = "";
		action_massive[action_n] = "";
		std::cout << "\033[40;37m\033[2;2H" << "               " << "\033[11;1H" << std::flush;
        }
	if(but_num_pos_1 == 3 && but_num_pos_2 == 2){
		std::string command_str;
		for(int count_1 = 0; count_1 <= number_n; count_1++){
			command_str += number_massive[count_1]+action_massive[count_1];
		}
		std::string com_bc_str = "echo " + command_str + " | bc";
		char * com_bc = new char[com_bc_str.length()+1];
		strcpy(com_bc, com_bc_str.c_str());
		std::cout << "\033[40;37m\033[2;2H" << "               " << "\033[2;2H" << std::flush << std::flush;
		system(com_bc);
		std::cout << "\033[11;1H" << std::flush;
	}
}

int main(){
	std::cout << "\033[H\033[2J" << std::flush;
	for(int brush = 0; brush <= 11; brush++){
		std::cout << "\033[46;37m\033[" << brush << ";0H" << "                 " << std::flush;
	}
	std::cout << "\033[40;37m\033[2;2H" << "               " << std::flush;
	set_keypress();
	select_button(0, 0);
	int selected_pos_1 = 0;
	int selected_pos_2 = 0;
	while(true){
		switch(int symbol = getchar()){
			case 67:
				selected_pos_2 += 1;
				if(selected_pos_2 > 3){
			              	selected_pos_2 = 0;
				}
		                select_button(selected_pos_1, selected_pos_2);
				break;
			case 68:
				selected_pos_2 -= 1;
				if(selected_pos_2 < 0){
				        selected_pos_2 = 3;
				}
				select_button(selected_pos_1, selected_pos_2);
				break;
			case 65:
				selected_pos_1 -= 1;
				if(selected_pos_1 < 0){
					selected_pos_1 = 3;
				}
				select_button(selected_pos_1, selected_pos_2);
				break;
			case 66:
				selected_pos_1 += 1;
				if(selected_pos_1 > 3){
					selected_pos_1 = 0;
				}
				select_button(selected_pos_1, selected_pos_2);
				break;
			case 10:
				number_action(selected_pos_1, selected_pos_2);
				break;
		}
	}
	std::cout << std::endl;
	return 0;
}
