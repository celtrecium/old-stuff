#include <stdio.h>
#include "winlib.h"

int main(void) {
    window root_win;
    window main_win;
    window win_1;
    window win_2;
    window win_3;

    colsch win_cs = create_cs(6, 7);
    colsch win_1_cs = create_cs(3, 7);
    colsch win_2_cs = create_cs(220, 199);
    colsch win_3_cs = create_cs(2, 0);
    colsch text_cs = create_cs(154, 0);
    colsch rect = create_cs(172, 33);

    box_symbols b1_sym = set_box_symbols("│", "│", "─", "─", "┌", "┐", "└", "┘");

    init_win(&root_win);

    main_win = create_window(&root_win, &win_cs, 10, 5, 91, 30); 
    win_1 = create_window(&main_win, &win_1_cs, 1, 1, 44, 28);
    win_2 = create_window(&main_win, &win_2_cs, 36, 6, 44, 14);
    win_3 = create_window(&main_win, &win_3_cs, 46, 16, 44, 13);

    window_fill(&main_win); 
    window_fill(&win_1);
    window_fill(&win_2);
    window_fill(&win_3);

    window_rectangle(&win_2, &rect, 10, 3, 45, 6);
    window_line(&win_2, &rect, 1, 2, 43, 19);

    window_box(&main_win, &win_cs, &b1_sym, 0, 0, main_win.width, main_win.height);
    window_box(&win_1, &win_1_cs, &b1_sym, 0, 0, win_1.width, win_1.height);
    window_box(&win_2, &win_2_cs, &b1_sym, 0, 0, win_2.width, win_2.height);
    window_box(&win_3, &win_3_cs, &b1_sym, 0, 0, win_3.width, win_3.height);
    window_printxy(&win_2, &text_cs, 25, 4, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");

    end_win();

    return(0);
}
