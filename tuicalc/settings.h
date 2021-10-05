/* This header file contains Tuicalc settings */

#ifndef TUICALC_SETTINGS_H
#define TUICALC_SETTINGS_H

namespace COLOR{
	enum class BG{
		BLACK           = 40,
	        RED             = 41,
	        GREEN           = 42,
	        YELLOW          = 43,
	        BLUE            = 44,
	        MAGENTA         = 45,
	        CYAN            = 46,
	        WHITE           = 47,
        	DEFAULT         = 49,

	        BRIGHT_BLACK    = 100,
	        BRIGHT_RED      = 101,
	        BRIGHT_GREEN    = 102,
	        BRIGHT_YELLOW   = 103,
	        BRIGHT_BLUE     = 104,
	        BRIGHT_MAGENTA  = 105,
	        BRIGHT_CYAN     = 106,
	        BRIGHT_WHITE    = 107
	};
	enum class FG{
		BLACK           = 30,
	        RED             = 31,
	        GREEN           = 32,
	        YELLOW          = 33,
	        BLUE            = 34,
	        MAGENTA         = 35,
	        CYAN            = 36,
	        WHITE           = 37,
	
	        BRIGHT_BLACK    = 90,
	        BRIGHT_RED      = 91,
	        BRIGHT_GREEN    = 92,
	        BRIGHT_YELLOW   = 93,
	        BRIGHT_BLUE     = 94,
	        BRIGHT_MAGENTA  = 95,
	        BRIGHT_CYAN     = 96,
	        BRIGHT_WHITE    = 97
	};
};

int calc_color_bg{static_cast<int>(COLOR::BG::CYAN)};
int calc_color_fg{static_cast<int>(COLOR::FG::WHITE)};
int button_color_bg{static_cast<int>(COLOR::BG::BRIGHT_WHITE)};
int button_color_fg{static_cast<int>(COLOR::FG::BLACK)};
int button_color_active_bg{static_cast<int>(COLOR::BG::BRIGHT_CYAN)};
int button_color_active_fg{static_cast<int>(COLOR::FG::BLACK)};
int num_screen_bg{static_cast<int>(COLOR::BG::BLACK)};
int num_screen_fg{static_cast<int>(COLOR::FG::WHITE)};
int logo_bg{static_cast<int>(COLOR::BG::BRIGHT_CYAN)};
int logo_fg{static_cast<int>(COLOR::FG::BLACK)};

#endif /* TUICALC_SETTINGS_H */
