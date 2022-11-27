#pragma once

#include <curses.h>
#include <string>

#include "Snake.hpp"
#include "Fruit.hpp"
#include "ColorPair.hpp"
#include "Menu.hpp"
#include "Vector2i.hpp"

namespace tile_definitions {
	const char snake_head = '@';
	const char snake_dead = 'X';
	const char snake_body = 'o';
	const char border = '#';
	const char fruit = '*';
}

class Display {
	WINDOW* win;
	int COLOR_BACK; // background color
	Vector2i win_size;
	Vector2i term_size; // kept to check if it changed
	
	//to be initialized int colorInit()
	ColorPair color_snake;
	ColorPair color_dead;
	ColorPair color_border;
	ColorPair color_fruit;
	ColorPair color_scores;
	ColorPair color_menu_header;
	ColorPair color_menu_option;
	ColorPair color_menu_selected;

	Vector2i findCenteredPos();

public:
	Display(Vector2i win_size);
	~Display() { endwin(); }

	WINDOW* getWindow();

	void cursesInit();
	void colorInit();
	void windowInit();
	void printChar(Vector2i pos, char ch, ColorPair color=ColorPair());
	void printString(Vector2i pos, std::string_view str, ColorPair color=ColorPair());
	void printGame(Snake snake, const std::vector<Fruit>& fruits);
	void printMenu(Menu menu);
	void printDead(Vector2i snake_pos);
	void checkTermSize();
};
