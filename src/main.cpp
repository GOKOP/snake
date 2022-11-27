#include <iostream>
#include <curses.h>
#include <string_view>
#include <unistd.h>
#include <optional>
#include <charconv>
#include <thread>
#include <chrono>

#include "Snake.hpp"
#include "Display.hpp"
#include "FruitManager.hpp"
#include "enums.hpp"
#include "Menu.hpp"
#include "globals.hpp"
#include "Vector2i.hpp"

void handleOptions(int argc, char* argv[], Vector2i& win_size, int& snake_delay, int& min_fruits, int& beg_length);
void printHelp();
Vector2i setWinSize(std::string_view size_str);
int setSnakeDelay(std::string_view delay_str);
int setMinFruits(std::string_view num_str);
int setBegLength(std::string_view len_str);
void gameReset(Snake& snake, FruitManager& fruit_manager, Vector2i win_size, int beg_len);
GameState advanceGame(Snake &snake, Vector2i win_size, FruitManager& fruit_manager);
void processGameInput(WINDOW* win, Snake& snake, GameState& state);
void processMenuInput(WINDOW* win, Menu& menu, GameState& state);
void clearInput(WINDOW* win);
Menu initMainMenu();

int main(int argc, char* argv[]) {
	Vector2i win_size{0, 0};
	int snake_delay = 0;
	int min_fruits  = 0;
	int beg_length  = 0;

	handleOptions(argc, argv, win_size, snake_delay, min_fruits, beg_length);

	if(snake_delay == 0) snake_delay = 100;
	if(win_size == Vector2i{0,0}) {
		win_size = {default_config::width, default_config::height};
	}
	if(min_fruits  == 0) min_fruits = default_config::min_fruits;
	if(beg_length  == 0) beg_length = default_config::snake_length;

	srand(time(NULL));
	
	Display display(win_size);

	Menu main_menu = initMainMenu();
	Snake snake({win_size.x/2, win_size.y/2}, Direction::Right, beg_length);
	FruitManager fruit_manager(min_fruits);
	GameState state = GameState::MainMenu;

	bool running = true;
	
	while(running) {
		display.checkTermSize();

		if(state == GameState::MainMenu) {
			processMenuInput(display.getWindow(), main_menu, state);
			display.printMenu(main_menu);
			// ensures the loop doesn't eat 100% of a cpu core pointlessly
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		else if(state == GameState::Lost) {
			display.printDead(snake.getHeadPos());
			std::this_thread::sleep_for(std::chrono::seconds(2));
			clearInput(display.getWindow());
			gameReset(snake, fruit_manager, win_size, beg_length);
			state = GameState::MainMenu;
		}
		else if(state == GameState::Canceled) {
			gameReset(snake, fruit_manager, win_size, beg_length);
			state = GameState::MainMenu;
		}
		else if(state == GameState::Running) {
			// weird order ensures that state changed by processGameInput() won't be overwritten by advanceGame()
			state = advanceGame(snake, win_size, fruit_manager);
			display.printGame(snake, fruit_manager.getFruits());
			std::this_thread::sleep_for(std::chrono::milliseconds(snake_delay));
			processGameInput(display.getWindow(), snake, state);
		}
		else if(state == GameState::Quit) {
			running = false;
		}
	}

	return 0;
}

void handleOptions(int argc, char* argv[], Vector2i& win_size, int& snake_delay, int& min_fruits, int& beg_length) {
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

std::optional<int> stringToInt(std::string_view str) {
	int value;

	const char* begin = str.data();
	const char* end = str.data() + str.size();

	auto result = std::from_chars(begin, end, value);
	if(result.ec != std::errc() || result.ptr != end) return {};
	else return value;
}

Vector2i setWinSize(std::string_view size_str) {
	auto x_pos = size_str.find('x');
	if(x_pos == std::string_view::npos || x_pos == size_str.size() - 1) return {0, 0};

	auto width_str = size_str.substr(0, x_pos);
	auto height_str = size_str.substr(x_pos + 1);

	int width = stringToInt(width_str).value_or(0);
	int height = stringToInt(height_str).value_or(0);

	return {width, height};
}

int setSnakeDelay(std::string_view delay_str) {
	int delay = 0;

	auto ms_pos = delay_str.find("ms");

	// if given miliseconds, set the delay directly
	if(ms_pos != std::string_view::npos && ms_pos == delay_str.size() - 2) {
		delay_str.remove_suffix(2);
		delay = stringToInt(delay_str).value_or(0);
		if(delay < 0) delay = 0;
	} // else set based on predefined speed
	else if(delay_str == "1") delay = static_config::speed1_ms;
	else if(delay_str == "2") delay = static_config::speed2_ms;
	else if(delay_str == "3") delay = static_config::speed3_ms;
	else if(delay_str == "4") delay = static_config::speed4_ms;

	return delay;
}

int setMinFruits(std::string_view num_str) {
	int min_num = stringToInt(num_str).value_or(0);
	if(min_num<0) min_num = 0;
	return min_num;
}

int setBegLength(std::string_view len_str) {
	int len_num = stringToInt(len_str).value_or(0);
	if(len_num<0) len_num = 0;
	return len_num;
}

void gameReset(Snake& snake, FruitManager& fruit_manager, Vector2i win_size, int beg_len) {
	int min_fruits = fruit_manager.getMinFruits();

	snake          = Snake({win_size.x/2, win_size.y/2}, Direction::Right, beg_len);
	fruit_manager  = FruitManager(min_fruits);
}

GameState advanceGame(Snake &snake, Vector2i win_size, FruitManager& fruit_manager) {
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
		return GameState::Lost;
	else {
		for(int i=1; i<snake.getBodySize(); ++i) {
			if(snake.getHeadPos() == snake.getBodyPiecePos(i))
				return GameState::Lost;
		}
	}

	return GameState::Running;
}

void processGameInput(WINDOW* win, Snake& snake, GameState& state) {
	switch(tolower(wgetch(win))) {
		case KEY_UP:
		case 'w':
		case 'k':
			if(snake.getDirection() != Direction::Down) snake.turn(Direction::Up); break;
		case KEY_DOWN:
		case 's':
		case 'j':
			if(snake.getDirection() != Direction::Up) snake.turn(Direction::Down); break;
		case KEY_LEFT:
		case 'a':
		case 'h':
			if(snake.getDirection() != Direction::Right) snake.turn(Direction::Left); break;
		case KEY_RIGHT:
		case 'd':
		case 'l':
			if(snake.getDirection() != Direction::Left) snake.turn(Direction::Right); break;
		case 'q':
			state = GameState::Canceled;
		default:
			break;
	}
}

void processMenuInput(WINDOW* win, Menu& menu, GameState& state) {
	switch(tolower(wgetch(win))) {
		case KEY_UP:
		case 'w':
		case 'k':
			menu.moveSelection(Direction::Up); break;
		case KEY_DOWN:
		case 's':
		case 'j':
			menu.moveSelection(Direction::Down); break;
		case '\n':
		case KEY_RIGHT:
		case 'd':
		case 'l':
			state = menu.getOption(menu.getSelection()).target_state; break;
		case 'q':
			state = GameState::Quit;
	}
}

void clearInput(WINDOW* win) {
	while(wgetch(win) != ERR) {};
}

Menu initMainMenu() {
	Menu menu("[cool name here]", static_config::version);
	menu.addOption(MenuOption{"Start Game", GameState::Running});
	menu.addOption(MenuOption{"Quit", GameState::Quit});

	return menu;
}
