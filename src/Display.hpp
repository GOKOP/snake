#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <curses.h>
#include "IntPair.hpp"
#include "Snake.hpp"
#include "Fruit.hpp"
#include "ColorPair.hpp"

// macros for used characters
#define SNAKE_HEAD '@'
#define SNAKE_DEAD 'X'
#define SNAKE_BODY 'o'
#define BORDER '#'
#define FRUIT '*'

class Display {
	WINDOW* win;
	int COLOR_BACK; // background color, -1 if default colors possible, otherwise COLOR_BLACK
	
	//to be initialized int colorInit()
	ColorPair* color_snake;
	ColorPair* color_dead;
	ColorPair* color_border;
	ColorPair* color_fruit;

	IntPair findCenteredPos(IntPair win_size, IntPair term_size);

public:
	Display(IntPair win_size);

	WINDOW* getWindow();

	void cursesInit();
	void colorInit();
	void windowInit(IntPair win_size);
	void printChar(IntPair pos, char ch, ColorPair* color=NULL);
	void printWin(Snake snake, std::vector<Fruit> fruits);
	void printDead(IntPair snake_pos);
};

#endif
