#pragma once

#include <curses.h>
#include <string>
#include <utility>

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
	std::pair<int, int> win_size;
	std::pair<int, int> term_size; // kept to check if it changed
	
	//to be initialized int colorInit()
	ColorPair* color_snake;
	ColorPair* color_dead;
	ColorPair* color_border;
	ColorPair* color_fruit;
	ColorPair* color_scores;
	ColorPair* color_menu_header;
	ColorPair* color_menu_option;
	ColorPair* color_menu_selected;

	std::pair<int, int> findCenteredPos();

public:
	Display(std::pair<int, int> win_size);

	WINDOW* getWindow();

	void cursesInit();
	void colorInit();
	void windowInit();
	void printChar(std::pair<int, int> pos, char ch, ColorPair* color=NULL);
	void printString(std::pair<int, int> pos, std::string str, ColorPair* color=NULL);
	void printGame(Snake snake, std::vector<Fruit> fruits);
	void printMenu(Menu menu);
	void printDead(std::pair<int, int> snake_pos);
	void checkTermSize();
};
