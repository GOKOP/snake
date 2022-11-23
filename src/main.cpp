#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctype.h>
#include <string>
#include <utility>

#include "Snake.hpp"
#include "Display.hpp"
#include "FruitManager.hpp"
#include "Enums.hpp"
#include "Menu.hpp"
#include "globals.hpp"

void handleOptions(int argc, char* argv[], std::pair<int, int>& win_size, int& snake_delay, int& min_fruits, int& beg_length);
void printHelp();
std::pair<int, int> setWinSize(char* size_str);
int setSnakeDelay(char* delay_str);
int setMinFruits(char* num_str);
int setBegLength(char* len_str);
void millisleep(int millisec);
void gameReset(Snake& snake, FruitManager& fruit_manager, std::pair<int, int> win_size, int beg_len);
GameState advanceGame(Snake &snake, std::pair<int, int> win_size, FruitManager& fruit_manager);
void processGameInput(WINDOW* win, Snake& snake, GameState& state);
void processMenuInput(WINDOW* win, Menu& menu, GameState& state);
void clearInput(WINDOW* win);
Menu initMainMenu();

int main(int argc, char* argv[]) {
	std::pair<int, int> win_size;
	int snake_delay = 0;
	int min_fruits  = 0;
	int beg_length  = 0;

	handleOptions(argc, argv, win_size, snake_delay, min_fruits, beg_length);

	if(snake_delay == 0) snake_delay = 100;
	if(win_size == std::pair<int, int>(0,0)) {
		win_size = {default_config::width, default_config::height};
	}
	if(min_fruits  == 0) min_fruits = default_config::min_fruits;
	if(beg_length  == 0) beg_length = default_config::snake_length;

	srand(time(NULL));
	
	Display display(win_size);

	Menu main_menu = initMainMenu();
	Snake snake({win_size.first/2, win_size.second/2}, RIGHT, beg_length);
	FruitManager fruit_manager(min_fruits);
	GameState state = MAIN_MENU;
	
	while(true) {
		display.checkTermSize();

		if(state == MAIN_MENU) {
			processMenuInput(display.getWindow(), main_menu, state);
			display.printMenu(main_menu);
			millisleep(10); // ensures that the loop won't pointlessly take the whole cpu
		}
		else if(state == LOST) {
			display.printDead(snake.getHeadPos());
			millisleep(2000);
			clearInput(display.getWindow());
			gameReset(snake, fruit_manager, win_size, beg_length);
			state = MAIN_MENU;
		}
		else if(state == CANCELED) {
			gameReset(snake, fruit_manager, win_size, beg_length);
			state = MAIN_MENU;
		}
		else if(state == RUNNING) {
			// weird order ensures that state changed by processGameInput() won't be overwritten by advanceGame()
			state = advanceGame(snake, win_size, fruit_manager);
			display.printGame(snake, fruit_manager.getFruits());
			millisleep(snake_delay);
			processGameInput(display.getWindow(), snake, state);
		}
		else if(state == QUIT) {
			endwin();
			exit(0);
		}
	}

	endwin();
	return 0;
}

void handleOptions(int argc, char* argv[], std::pair<int, int>& win_size, int& snake_delay, int& min_fruits, int& beg_length) {
	int opt;
	while( (opt = getopt(argc, argv, "hw:s:f:l:")) != -1 ) {
		switch(opt) {
			case 'w':
				if(optarg) win_size = setWinSize(optarg); break;
			case 's':
				if(optarg) snake_delay = setSnakeDelay(optarg); break;
			case 'f':
				if(optarg) min_fruits = setMinFruits(optarg); break;
			case 'l':
				if(optarg) beg_length = setBegLength(optarg); break;
			case 'h': 
			default: printHelp(); break;
		}
	}
}
void printHelp() {
	std::cout<<"Snake game written in ncurses, configurable through command line options.\n";
	std::cout<<"Available options are:\n";
	std::cout<<" -h\tprint this message\n";
	std::cout<<" -w XxY\tchange game window's width to X and height to Y (default: "<<default_config::width<<"x"<<default_config::height<<")\n";
	std::cout<<" -s S\tchange snake's speed. S can be a number from 1 to 4 or a number of\n";
	std::cout<<" \tmilliseconds (appended with \"ms\") between snake moves (default: 2 or 10ms)\n";
	std::cout<<" -f N\tminimal number of fruits at one time. When there's less fruits than N,\n";
	std::cout<<" \tthe game will spawn a new one each step until there's enough. (default: 1)\n";
	std::cout<<" -l L\tchange initial length of the snake. L must be a positive integer number.\n";
	std::cout<<"\t This is the length without head. Eg. `-l 1` will result in a snake with a head\n";
	std::cout<<"\t*and* one body element.\n";
	std::cout<<"If incorrect values are given, defaults will be used.\n";
	std::cout<<"Note: all numbers given to above options must be positive integers.\n";

	std::cout<<"In game controls:\n";
	std::cout<<" arrows, wasd, hjkl\n";
	std::cout<<"\tmovement (both in game and menu)\n";
	std::cout<<" enter, right arrow, d, l\n";
	std::cout<<"\tselect option (in menu)\n";
	std::cout<<" q\n";
	std::cout<<"\tquit to menu (in game), exit game (in menu)\n";
	exit(0);
}

std::pair<int, int> setWinSize(char* size_str) {
	std::string width_str = "";
	std::string height_str = "";
	bool read_width = true;

	int i = 0;
	while(size_str[i] != '\0') {
		if(size_str[i] == 'x') read_width = false;
		else if(read_width) width_str += size_str[i];
		else height_str += size_str[i];
		++i;
	}

	int width  = 0;
	int height = 0;

	try {
		width  = std::stoi(width_str);
		height = std::stoi(height_str);
	} catch(...){}; // no catching needed here

	return {width, height};
}

void millisleep(int millisec) {
	usleep(millisec*1000);
}

int setSnakeDelay(char* delay_str) {
	int delay = 0;

	std::string delay_string(delay_str);
	size_t size = delay_string.size();
	// if given miliseconds, set the delay directly
	if(size >= 3 && delay_string[size-2]=='m' && delay_string[size-1]=='s') {
		delay_string.erase(size-2);

		try {
			delay = std::stoi(delay_string);
		} catch(...){};

		if(delay<0) delay = 0;
	} // else set based on predefined speed
	else if(delay_string == "1") delay = fixed_config::speed1_ms;
	else if(delay_string == "2") delay = fixed_config::speed2_ms;
	else if(delay_string == "3") delay = fixed_config::speed3_ms;
	else if(delay_string == "4") delay = fixed_config::speed4_ms;

	return delay;
}

int setMinFruits(char* num_str) {
	int min_num = 0;
	std::string num_string(num_str);
	
	try {
		min_num = std::stoi(num_string);
	} catch(...){};
	
	if(min_num<0) min_num = 0;

	return min_num;
}

int setBegLength(char* len_str) {
	int len_num = 0;
	std::string len_string(len_str);

	try {
		len_num = std::stoi(len_str);
	} catch(...){};

	if(len_num<0) len_num = 0;

	return len_num;
}

void gameReset(Snake& snake, FruitManager& fruit_manager, std::pair<int, int> win_size, int beg_len) {
	int min_fruits = fruit_manager.getMinFruits();

	snake          = Snake({win_size.first/2, win_size.second/2}, RIGHT, beg_len);
	fruit_manager  = FruitManager(min_fruits);
}

GameState advanceGame(Snake &snake, std::pair<int, int> win_size, FruitManager& fruit_manager) {
	if(fruit_manager.getFruits().size()<fruit_manager.getMinFruits()) {
		fruit_manager.add(win_size);
		
		bool conflicts = false;
		do {
			conflicts = false;
			for(int i=0; i<snake.getBodySize(); ++i) {
				if(fruit_manager.getFruit(fruit_manager.getFruitCount()-1).pos == snake.getBodyPiecePos(i))
					conflicts = true;
			}
			if(conflicts) {
				fruit_manager.remove(fruit_manager.getFruitCount()-1);
				fruit_manager.add(win_size);
			}
		} while(conflicts);
	}

	snake.advancePos();	

	for(int i=0; i<fruit_manager.getFruits().size(); ++i) {
		if(snake.getHeadPos() == fruit_manager.getFruits()[i].pos) {
			snake.feed(fruit_manager.getFruits()[i].value);
			fruit_manager.remove(i);
		}
	}

	if(snake.getHeadPos().first==0 || snake.getHeadPos().second==0 ||
	   snake.getHeadPos().first==win_size.first-1 || snake.getHeadPos().second==win_size.second-1)
		return LOST;
	else {
		for(int i=1; i<snake.getBodySize(); ++i) {
			if(snake.getHeadPos() == snake.getBodyPiecePos(i))
				return LOST;
		}
	}

	return RUNNING;
}

void processGameInput(WINDOW* win, Snake& snake, GameState& state) {
	switch(tolower(wgetch(win))) {
		case KEY_UP:
		case 'w':
		case 'k':
			if(snake.getDirection() != DOWN) snake.turn(UP); break;
		case KEY_DOWN:
		case 's':
		case 'j':
			if(snake.getDirection() != UP) snake.turn(DOWN); break;
		case KEY_LEFT:
		case 'a':
		case 'h':
			if(snake.getDirection() != RIGHT) snake.turn(LEFT); break;
		case KEY_RIGHT:
		case 'd':
		case 'l':
			if(snake.getDirection() != LEFT) snake.turn(RIGHT); break;
		case 'q':
			state = CANCELED;
		default:
			break;
	}
}

void processMenuInput(WINDOW* win, Menu& menu, GameState& state) {
	switch(tolower(wgetch(win))) {
		case KEY_UP:
		case 'w':
		case 'k':
			menu.moveSelection(UP); break;
		case KEY_DOWN:
		case 's':
		case 'j':
			menu.moveSelection(DOWN); break;
		case '\n':
		case KEY_RIGHT:
		case 'd':
		case 'l':
			state = menu.getOption(menu.getSelection()).target_state; break;
		case 'q':
			state = QUIT;
	}
}

void clearInput(WINDOW* win) {
	while(wgetch(win) != ERR) {};
}

Menu initMainMenu() {
	Menu menu("[cool name here]", fixed_config::version);
	menu.addOption(MenuOption("Start Game", RUNNING));
	menu.addOption(MenuOption("Quit", QUIT));

	return menu;
}
