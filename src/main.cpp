#include <curses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctype.h>

#include "IntPair.hpp"
#include "Snake.hpp"
#include "Display.hpp"
#include "FruitManager.hpp"
#include "Enums.hpp"
#include "Menu.hpp"

# define MIN_FRUITS 1

void millisleep(int millisec);
void gameReset(Snake& snake, FruitManager& fruit_manager, IntPair win_size);
GameState advanceGame(Snake &snake, IntPair win_size, FruitManager& fruit_manager);
void processGameInput(WINDOW* win, Snake& snake);
void processMenuInput(WINDOW* win, Menu& menu, GameState& state);
Menu initMainMenu();

int main() {
	srand(time(NULL));
	
	IntPair win_size(40,20);
	Display display(win_size);

	Menu main_menu = initMainMenu();
	Snake snake(IntPair(win_size.x/2, win_size.y/2), RIGHT);
	FruitManager fruit_manager;
	GameState state = MAIN_MENU;
	
	while(true) {
		if(state == MAIN_MENU) {
			processMenuInput(display.getWindow(), main_menu, state);
			display.printMenu(main_menu);
		}
		else if(state == RUNNING) {
			processGameInput(display.getWindow(), snake);
			state = advanceGame(snake, win_size, fruit_manager);
			display.printGame(snake, fruit_manager.getFruits());
			millisleep(10);
		}
		else if(state == QUIT) {
			endwin();
			return 0;
		}
		
		if(state == LOST) {
			display.printDead(snake.getHeadPos());
			millisleep(200);
			gameReset(snake, fruit_manager, win_size);
			state = MAIN_MENU;
		}
	}

	endwin();
	return 0;
}

void millisleep(int millisec) {
	usleep(millisec*10000);
}

void gameReset(Snake& snake, FruitManager& fruit_manager, IntPair win_size) {
	snake 		   = Snake(IntPair(win_size.x/2, win_size.y/2), RIGHT);
	fruit_manager = FruitManager();
}

GameState advanceGame(Snake &snake, IntPair win_size, FruitManager& fruit_manager) {
	if(fruit_manager.getFruits().size()<MIN_FRUITS) {
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

void processGameInput(WINDOW* win, Snake& snake) {
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
			state = menu.getOption(menu.getSelection()).target_state;
	}
}

Menu initMainMenu() {
	Menu menu("[cool name here]");
	menu.addOption(MenuOption("Start Game", RUNNING));
	menu.addOption(MenuOption("Quit", QUIT));

	return menu;
}
