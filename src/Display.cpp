#include "Display.hpp"

Display::Display(IntPair win_size) {
	cursesInit();
	colorInit();
	windowInit(win_size);
}

WINDOW* Display::getWindow() {
	return win;
}

void Display::cursesInit() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
}

void Display::colorInit() {
	if(has_colors()) {
		start_color();

		if(use_default_colors() != ERR)
			COLOR_BACK = -1; // ncurses default term color
		else
			COLOR_BACK = COLOR_BLACK;

		color_snake  = new ColorPair(1, COLOR_GREEN, COLOR_BACK, true);
		color_dead   = new ColorPair(2, COLOR_RED, COLOR_BACK, true);
		color_border = new ColorPair(3, COLOR_WHITE, COLOR_WHITE);
		color_fruit  = new ColorPair(4, COLOR_RED, COLOR_BACK);
	}
}

void Display::windowInit(IntPair win_size) {
	IntPair term_size; 
	getmaxyx(stdscr, term_size.y, term_size.x);

	IntPair win_pos = findCenteredPos(win_size, term_size);
	win = newwin(win_size.y, win_size.x, win_pos.y, win_pos.x);
	nodelay(win, TRUE);
	keypad(win, TRUE);
}

IntPair Display::findCenteredPos(IntPair win_size, IntPair term_size) {
	IntPair centered_pos;
	centered_pos.x = term_size.x/2 - win_size.x/2;
	centered_pos.y = term_size.y/2 - win_size.y/2;

	return centered_pos;
}

void Display::printChar(IntPair pos, char ch, ColorPair* color) {
	IntPair max_pos;
	getmaxyx(win, max_pos.y, max_pos.x);

	if(color!=NULL) color->enable(win);

	if(pos.x>=0 && pos.x<max_pos.x && pos.y>=0 && pos.y<max_pos.y) {
		wmove(win, pos.y, pos.x);
		waddch(win, ch);
	}

	if(color!=NULL) color->disable(win);
}

void Display::printGame(Snake snake, std::vector<Fruit> fruits) {
	werase(win);

	color_border->enable(win);
	wborder(win, BORDER, BORDER, BORDER, BORDER, BORDER, BORDER, BORDER, BORDER);
	color_border->disable(win);

	for(int i=0; i<fruits.size(); ++i) {
		printChar(fruits[i].pos, FRUIT, color_fruit);
	}
	
	for(int i=0; i<snake.getBodySize(); ++i) {
		if(i==0)
			printChar(snake.getBodyPiecePos(i), SNAKE_HEAD, color_snake);
		else 
			printChar(snake.getBodyPiecePos(i), SNAKE_BODY, color_snake); 
	} 
	wrefresh(win);
}

void Display::printDead(IntPair snake_pos) {
	printChar(snake_pos, SNAKE_DEAD, color_dead);
	wrefresh(win);
}
