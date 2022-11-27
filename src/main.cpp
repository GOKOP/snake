#include <iostream>
#include <curses.h>
#include <thread>
#include <chrono>

#include "Snake.hpp"
#include "Display.hpp"
#include "FruitManager.hpp"
#include "enums.hpp"
#include "Menu.hpp"
#include "globals.hpp"
#include "Vector2i.hpp"
#include "CommandConfig.hpp"

void printHelp();
void gameReset(Snake& snake, FruitManager& fruit_manager, const CommandConfig& config);
GameState advanceGame(Snake &snake, Vector2i win_size, FruitManager& fruit_manager);
void processGameInput(WINDOW* win, Snake& snake, GameState& state);
void processMenuInput(WINDOW* win, Menu& menu, GameState& state);
void clearInput(WINDOW* win);
Menu initMainMenu();

int main(int argc, char* argv[]) {
	CommandConfig command_config(argc, argv);
	if(!command_config.isOk()) {
		printHelp();
		exit(1);
	}

	srand(time(NULL));
	
	Display display(command_config.getWindowSize());

	Menu main_menu = initMainMenu();

	Snake snake(
			{command_config.getWindowSize().x/2, command_config.getWindowSize().y/2},
			Direction::Right,
			command_config.getBegLength()
	);

	FruitManager fruit_manager(command_config.getMinFruits());
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
			gameReset(snake, fruit_manager, command_config);
			state = GameState::MainMenu;
		}
		else if(state == GameState::Canceled) {
			gameReset(snake, fruit_manager, command_config);
			state = GameState::MainMenu;
		}
		else if(state == GameState::Running) {
			// weird order ensures that state changed by processGameInput() won't be overwritten by advanceGame()
			state = advanceGame(snake, command_config.getWindowSize(), fruit_manager);
			display.printGame(snake, fruit_manager.getFruits());
			std::this_thread::sleep_for(std::chrono::milliseconds(command_config.getSnakeDelay()));
			processGameInput(display.getWindow(), snake, state);
		}
		else if(state == GameState::Quit) {
			running = false;
		}
	}

	return 0;
}

void printHelp() {
	std::cout<<"Snake game written in ncurses, configurable through command line options.\n";
	std::cout<<"Available options are:\n";
	std::cout<<" -h\tprint this message\n";
	std::cout<<" -w XxY\tchange game window's width to X and height to Y (default: "<<default_config::window_size.x<<"x"<<default_config::window_size.y<<")\n";
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

void gameReset(Snake& snake, FruitManager& fruit_manager, const CommandConfig& config) {
	int min_fruits = fruit_manager.getMinFruits();

	snake = Snake(
			{config.getWindowSize().x/2, config.getWindowSize().y/2},
			Direction::Right,
			config.getBegLength()
	);
	fruit_manager = FruitManager(min_fruits);
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
