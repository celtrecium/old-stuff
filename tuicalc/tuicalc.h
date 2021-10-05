/* This is header of TUICalc */
#ifndef TUICALC_H
#define TUICALC_H

namespace tcalc{
	const int pic_heigth { 8 };
	std::string pic[pic_heigth] { 
		"           TUICalc",
		"         bc frontend",
		"          h - help",
		"",
		"         ▄▀▄     ▄▀▄",
		"        ▄█░░▀▀▀▀▀░░█▄",
		"    ▄▄  █░░░░░░░░░░░█  ▄▄",
		"   █▄▄█ █░░▀░░┬░░▀░░█ █▄▄█"
	 };

 	 const int help_heigth { 9 };	
	 std::string help[help_heigth] { 
		"        Help screen",
		" c - clear",
		" s - sqrt",
		" d - sin",
		" v - cos",
		" q - quit",
		"",
		"",
		" b - back to main screen"
	  };

}

#endif /* TUICALC_H */
