#include "Display.hpp"

Display::Display(std::pair<int, int> new_win_size) {
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
	getmaxyx(stdscr, term_size.second, term_size.first);

	if(term_size.first<win_size.first || term_size.second<win_size.second) {
		win = newwin(term_size.first, term_size.second, 0, 0);
		printString({0,0}, "PLS MAKE TERM BIGGER");
	}
	else {
		std::pair<int, int> win_pos = findCenteredPos();
		win = newwin(win_size.second, win_size.first, win_pos.second, win_pos.first);
		nodelay(win, TRUE);
		keypad(win, TRUE);
	}
}

std::pair<int, int> Display::findCenteredPos() {
	std::pair<int, int> centered_pos;
	centered_pos.first = term_size.first/2 - win_size.first/2;
	centered_pos.second = term_size.second/2 - win_size.second/2;

	return centered_pos;
}

void Display::printChar(std::pair<int, int> pos, char ch, ColorPair* color) {
	std::pair<int, int> max_pos;
	getmaxyx(win, max_pos.second, max_pos.first);

	if(color!=nullptr) color->enable(win);

	if(pos.first>=0 && pos.first<max_pos.first && pos.second>=0 && pos.second<max_pos.second) {
		wmove(win, pos.second, pos.first);
		waddch(win, ch);
	}

	if(color!=nullptr) color->disable(win);
}

void Display::printString(std::pair<int, int> pos, std::string str, ColorPair* color) {
	std::pair<int, int> max_pos;
	getmaxyx(win, max_pos.second, max_pos.first);

	if(color!=nullptr) color->enable(win);

	if(pos.first>=0 && pos.first+str.size()<max_pos.first && pos.second>=0 && pos.second<max_pos.second) {
		wmove(win, pos.second, pos.first);
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

	printString({1, win_size.second-1}, "Body: " + std::to_string( snake.getBodySize() ), color_scores);

	wrefresh(win);
}

void Display::printDead(std::pair<int, int> snake_pos) {
	printChar(snake_pos, SNAKE_DEAD, color_dead);
	wrefresh(win);
}

void Display::printMenu(Menu menu) {
	werase(win);
	
	int menu_height = menu.getOptionCount()+2; // options + header and an empty line
	if(menu_height > win_size.second) {
		waddstr(win, "win too small");
		return;
	}
	
	int menu_offset   = (win_size.second - menu_height)/2;             // for vertical centering
	int header_offset = (win_size.first - menu.getHeader().size())/2; // for horizontal centering

	wmove(win, menu_offset, header_offset);

	color_menu_header->enable(win);
	waddstr(win, menu.getHeader().data());
	color_menu_header->disable(win);

	for(int i=0; i<menu.getOptionCount(); ++i) {
		int option_offset = (win_size.first - menu.getOption(i).name.size())/2;
		wmove(win, menu_offset+2+i, option_offset);

		ColorPair* color;
		if(menu.getSelection() == i) color = color_menu_selected;
		else                         color = color_menu_option;

		color->enable(win);
		waddstr(win, menu.getOption(i).name.c_str());
		color->disable(win);
	}

	int corner_offset = (win_size.first - menu.getCornerText().size()); // for right align
	wmove(win, win_size.second-1, corner_offset);
	waddstr(win, menu.getCornerText().data());
}

void Display::checkTermSize() {
	std::pair<int, int> new_term;
	getmaxyx(stdscr, new_term.second, new_term.first);
	if(new_term != term_size) {
		erase();
		refresh();
		windowInit();
	}
}
