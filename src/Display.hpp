#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <curses.h>
#include "IntPair.hpp"
#include "Snake.hpp"

#define SNAKE_HEAD '@'
#define SNAKE_DEAD 'X'
#define SNAKE_BODY 'O'
#define BORDER '#'

class Display {
	WINDOW* win;

	IntPair findCenteredPos(IntPair win_size, IntPair term_size);

public:
	Display(IntPair win_size);

	WINDOW* getWindow();

	void cursesInit();
	void windowInit(IntPair win_size);
	void printChar(IntPair pos, chtype ch);
	void printWin(Snake snake);
	void printDead(IntPair snake_pos);
};

#endif
