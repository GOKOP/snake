#include "Display.hpp"

Display::Display(IntPair new_win_size) {
	cursesInit();
	colorInit();
	win_size = new_win_size;
	windowInit();
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
		color_scores = new ColorPair(5, COLOR_BLACK, COLOR_WHITE);
		
		color_menu_header   = new ColorPair(6, COLOR_CYAN, COLOR_BACK, true);
		color_menu_option   = new ColorPair(7, COLOR_WHITE, COLOR_BACK, true);
		color_menu_selected = new ColorPair(8, COLOR_WHITE, COLOR_GREEN, true);
	}
}

void Display::windowInit() {
	getmaxyx(stdscr, term_size.y, term_size.x);

	if(term_size.x<win_size.x || term_size.y<win_size.y) {
		win = newwin(term_size.x, term_size.y, 0, 0);
		printString(IntPair(0,0), "PLS MAKE TERM BIGGER");
	}
	else {
		IntPair win_pos = findCenteredPos(win_size, term_size);
		win = newwin(win_size.y, win_size.x, win_pos.y, win_pos.x);
		nodelay(win, TRUE);
		keypad(win, TRUE);
	}
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

void Display::printString(IntPair pos, std::string str, ColorPair* color) {
	IntPair max_pos;
	getmaxyx(win, max_pos.y, max_pos.x);

	if(color!=NULL) color->enable(win);

	if(pos.x>=0 && pos.x+str.size()<max_pos.x && pos.y>=0 && pos.y<max_pos.y) {
		wmove(win, pos.y, pos.x);
		waddstr(win, str.c_str());
	}
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

	printString(IntPair(1, win_size.y-1), "Body: " + std::to_string( snake.getBodySize() ), color_scores);

	wrefresh(win);
}

void Display::printDead(IntPair snake_pos) {
	printChar(snake_pos, SNAKE_DEAD, color_dead);
	wrefresh(win);
}

void Display::printMenu(Menu menu) {
	werase(win);
	
	int menu_height = menu.getOptionCount()+2; // options + header and an empty line
	if(menu_height > win_size.y) {
		waddstr(win, "win too small");
		return;
	}
	
	int menu_offset   = (win_size.y - menu_height)/2;		 	   // for vertical centering
	int header_offset = (win_size.x - menu.getHeader().size())/2; // for horizontal centering

	wmove(win, menu_offset, header_offset);

	color_menu_header->enable(win);
	waddstr(win, menu.getHeader().c_str());
	color_menu_header->disable(win);

	for(int i=0; i<menu.getOptionCount(); ++i) {
		int option_offset = (win_size.x - menu.getOption(i).name.size())/2;
		wmove(win, menu_offset+2+i, option_offset);

		ColorPair* color;
		if(menu.getSelection() == i) color = color_menu_selected;
		else					 	 color = color_menu_option;

		color->enable(win);
		waddstr(win, menu.getOption(i).name.c_str());
		color->disable(win);
	}

	int corner_offset = (win_size.x - menu.getCornerText().size()); // for right align
	wmove(win, win_size.y-1, corner_offset);
	waddstr(win, menu.getCornerText().c_str());
}

void Display::checkTermSize() {
	IntPair new_term;
	getmaxyx(stdscr, new_term.y, new_term.x);
	if(new_term != term_size) {
		erase();
		refresh();
		windowInit();
	}
}
