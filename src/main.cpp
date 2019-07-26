#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctype.h>
#include <string>

#include "IntPair.hpp"
#include "Snake.hpp"
#include "Display.hpp"
#include "FruitManager.hpp"
#include "Enums.hpp"
#include "Menu.hpp"

#define VERSION "v1.0"

#define DEF_MIN_FRUITS 1
#define DEF_WIDTH 40
#define DEF_HEIGHT 20

// delay in milliseconds
#define SPEED1 15
#define SPEED2 10
#define SPEED3 5
#define SPEED4 3

void handleOptions(int argc, char* argv[], IntPair& win_size, int& snake_delay, int& min_fruits);
void printHelp();
IntPair setWinSize(char* size_str);
int setSnakeDelay(char* delay_str);
int setMinFruits(char* num_str);
void millisleep(int millisec);
void gameReset(Snake& snake, FruitManager& fruit_manager, IntPair win_size);
GameState advanceGame(Snake &snake, IntPair win_size, FruitManager& fruit_manager);
void processGameInput(WINDOW* win, Snake& snake, GameState& state);
void processMenuInput(WINDOW* win, Menu& menu, GameState& state);
void clearInput(WINDOW* win);
Menu initMainMenu();

int main(int argc, char* argv[]) {
	IntPair win_size;
	int snake_delay = 0;
	int min_fruits  = 0;

	handleOptions(argc, argv, win_size, snake_delay, min_fruits);

	if(snake_delay == 0) snake_delay = 10;
	if(win_size    == IntPair(0,0)) win_size = IntPair(DEF_WIDTH,DEF_HEIGHT);
	if(min_fruits  == 0) min_fruits = DEF_MIN_FRUITS;

	srand(time(NULL));
	
	Display display(win_size);

	Menu main_menu = initMainMenu();
	Snake snake(IntPair(win_size.x/2, win_size.y/2), RIGHT);
	FruitManager fruit_manager(min_fruits);
	GameState state = MAIN_MENU;
	
	while(true) {
		display.checkTermSize();

		if(state == MAIN_MENU) {
			processMenuInput(display.getWindow(), main_menu, state);
			display.printMenu(main_menu);
			millisleep(1); // ensures that the loop won't pointlessly take the whole cpu
		}
		else if(state == LOST) {
			display.printDead(snake.getHeadPos());
			millisleep(200);
			clearInput(display.getWindow());
			gameReset(snake, fruit_manager, win_size);
			state = MAIN_MENU;
		}
		else if(state == CANCELED) {
			gameReset(snake, fruit_manager, win_size);
			state = MAIN_MENU;
		}
		else if(state == RUNNING) {
			// weird order ensures that state changed by processGameInput() won't be overwritten by advanceGame()
			state = advanceGame(snake, win_size, fruit_manager);
			millisleep(snake_delay);
			processGameInput(display.getWindow(), snake, state);
			display.printGame(snake, fruit_manager.getFruits());
		}
		else if(state == QUIT) {
			endwin();
			exit(0);
		}
	}

	endwin();
	return 0;
}

void handleOptions(int argc, char* argv[], IntPair& win_size, int& snake_delay, int& min_fruits) {
	int opt;
	while( (opt = getopt(argc, argv, "hw:s:f:")) != -1 ) {
		switch(opt) {
			case 'w':
				if(optarg) win_size = setWinSize(optarg); break;
			case 's':
				if(optarg) snake_delay = setSnakeDelay(optarg); break;
			case 'f':
				if(optarg) min_fruits = setMinFruits(optarg); break;
			case 'h': 
			default: printHelp(); break;
		}
	}
}
void printHelp() {
	std::cout<<"Snake game written in ncurses, configurable through command line options."<<std::endl;
	std::cout<<"Available options are:"<<std::endl;
	std::cout<<" -h\tprint this message"<<std::endl;
	std::cout<<" -w XxY\tchange game window's width to X and height to Y (default: "<<DEF_WIDTH<<"x"<<DEF_HEIGHT<<")"<<std::endl;
	std::cout<<" -s S\tchange snake's speed. S can be a number from 1 to 4 or a number of"<<std::endl;
	std::cout<<" \tmilliseconds (appended with \"ms\") between snake moves (default: 2 or 10ms)"<<std::endl;
	std::cout<<" -f N\tminimal number of fruits at one time. When there's less fruits than N,"<<std::endl;
	std::cout<<" \tthe game will spawn a new one each step until there's enough. (default: 1)"<<std::endl;
	std::cout<<"If incorrect values are given, defaults will be used."<<std::endl<<std::endl;

	std::cout<<"In game controls:"<<std::endl;
	std::cout<<" arrows, wasd, hjkl"<<std::endl;
	std::cout<<"\tmovement (both in game and menu)"<<std::endl;
	std::cout<<" enter, right arrow, d, l"<<std::endl;
	std::cout<<"\tselect option (in menu)"<<std::endl;
	std::cout<<" q"<<std::endl;
	std::cout<<"\tquit to menu (in game), exit game (in menu)"<<std::endl;
	exit(0);
}

IntPair setWinSize(char* size_str) {
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

	return IntPair(width, height);
}

void millisleep(int millisec) {
	usleep(millisec*10000);
}

int setSnakeDelay(char* delay_str) {
	int delay = 0;

	std::string delay_string(delay_str);
	size_t size = delay_string.size();
	// if given miliseconds, set the delay directly
	if(delay_string[size-2]=='m' && delay_string[size-1]=='s') {
		delay_string.erase(size-2);

		try {
			delay = std::stoi(delay_string);
		} catch(...){};
	} // else set based on predefined speed
	else if(delay_string == "1") delay = SPEED1;
	else if(delay_string == "2") delay = SPEED2;
	else if(delay_string == "3") delay = SPEED3;
	else if(delay_string == "4") delay = SPEED4;

	return delay;
}

int setMinFruits(char* num_str) {
	int min_num = 0;
	std::string num_string(num_str);
	
	try {
		min_num = std::stoi(num_string);
	} catch(...){};

	return min_num;
}

void gameReset(Snake& snake, FruitManager& fruit_manager, IntPair win_size) {
	int min_fruits = fruit_manager.getMinFruits();

	snake 		   = Snake(IntPair(win_size.x/2, win_size.y/2), RIGHT);
	fruit_manager  = FruitManager(min_fruits);
}

GameState advanceGame(Snake &snake, IntPair win_size, FruitManager& fruit_manager) {
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

	if(snake.getHeadPos().x==0 || snake.getHeadPos().y==0 ||
	   snake.getHeadPos().x==win_size.x-1 || snake.getHeadPos().y==win_size.y-1)
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
	Menu menu("[cool name here]", VERSION);
	menu.addOption(MenuOption("Start Game", RUNNING));
	menu.addOption(MenuOption("Quit", QUIT));

	return menu;
}
