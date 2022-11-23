#pragma once

#include <curses.h>

class ColorPair { // class to nicely manage ncurses colors with bold attribute n'shit
	int pair_num; 
	bool bold; // bold, that is usually lighter and not bold

public:
	ColorPair(int num, int fground, int bground, bool bold=false);
	ColorPair();
	void enable(WINDOW* win) const;
	void disable(WINDOW* win) const;
};
