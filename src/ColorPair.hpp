#ifndef COLORPAIR_HPP
#define COLORPAIR_HPP

#include <curses.h>

class ColorPair { // class to nicely manage ncurses colors with bold attribute n'shit
	int pair_num; 
	bool bold; // bold, that is usually lighter and not bold

public:
	ColorPair(int num, int fground, int bground, bool bold=false);
	void enable(WINDOW* win);
	void disable(WINDOW* win);
};

#endif
