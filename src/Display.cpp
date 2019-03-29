#include "Display.hpp"

Display::Display(IntPair win_size) {
	cursesInit();
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

void Display::printChar(IntPair pos, chtype ch) {
	IntPair max_pos;
	getmaxyx(win, max_pos.y, max_pos.x);

	if(pos.x>=0 && pos.x<max_pos.x && pos.y>=0 && pos.y<max_pos.y) {
		wmove(win, pos.y, pos.x);
		waddch(win, ch);
	}
}

void Display::printWin(Snake snake) {
	werase(win);
	wborder(win, BORDER, BORDER, BORDER, BORDER, BORDER, BORDER, BORDER, BORDER);
	
	for(int i=0; i<snake.getBodySize(); ++i) {
		if(i==0)
			printChar(snake.getBodyPiecePos(i), SNAKE_HEAD);
		else
			printChar(snake.getBodyPiecePos(i), SNAKE_BODY);
	}

	wrefresh(win);
}

void Display::printDead(IntPair snake_pos) {
	printChar(snake_pos, SNAKE_DEAD);
	wrefresh(win);
}
