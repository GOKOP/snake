#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <curses.h>
#include <string>

#include "IntPair.hpp"
#include "Snake.hpp"
#include "Fruit.hpp"
#include "ColorPair.hpp"
#include "Menu.hpp"

// macros for used characters
#define SNAKE_HEAD '@'
#define SNAKE_DEAD 'X'
#define SNAKE_BODY 'o'
#define BORDER '#'
#define FRUIT '*'

class Display {
	WINDOW* win;
	int COLOR_BACK; // background color
	IntPair win_size;
	IntPair term_size; // kept to check if it changed
	
	//to be initialized int colorInit()
	ColorPair* color_snake;
	ColorPair* color_dead;
	ColorPair* color_border;
	ColorPair* color_fruit;
	ColorPair* color_scores;
	ColorPair* color_menu_header;
	ColorPair* color_menu_option;
	ColorPair* color_menu_selected;

	IntPair findCenteredPos(IntPair win_size, IntPair term_size);

public:
	Display(IntPair win_size);

	WINDOW* getWindow();

	void cursesInit();
	void colorInit();
	void windowInit();
	void printChar(IntPair pos, wchar_t ch, ColorPair* color=NULL);
	void printString(IntPair pos, std::string str, ColorPair* color=NULL);
	void printString(IntPair pos, std::wstring str, ColorPair* color=NULL);
	void printGame(Snake snake, std::vector<Fruit> fruits);
	void printMenu(Menu menu);
	void printDead(IntPair snake_pos);
	void checkTermSize();
};

#endif
