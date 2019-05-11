#include <curses.h>
#include <unistd.h>
#include <cstdlib>

#include "IntPair.hpp"
#include "Snake.hpp"
#include "Display.hpp"
#include "FruitManager.hpp"

# define MIN_FRUITS 1

enum GameState {
	RUNNING,
	LOST
};

void millisleep(int millisec);
GameState advanceGame(Snake &snake, IntPair win_size, FruitManager& fruit_manager);
void processInput(WINDOW* win, Snake &snake);

int main() {
	srand(time(NULL));
	
	IntPair win_size(40,20);
	Display display(win_size);
	Snake snake(IntPair(20,10), RIGHT);
	FruitManager fruit_manager;
	GameState state = RUNNING;
	
	while(true) {
		processInput(display.getWindow(), snake);

		if(state == RUNNING) {
			state = advanceGame(snake, win_size, fruit_manager);
			display.printGame(snake, fruit_manager.getFruits());
		}
		
		if(state == LOST)
			display.printDead(snake.getHeadPos());

		millisleep(10);
	}

	endwin();
	return 0;
}

void millisleep(int millisec) {
	usleep(millisec*10000);
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

void processInput(WINDOW* win, Snake &snake) {
	switch(wgetch(win)) {
		case KEY_UP:
			if(snake.getDirection() != DOWN) snake.turn(UP); break;
		case KEY_DOWN:
			if(snake.getDirection() != UP) snake.turn(DOWN); break;
		case KEY_LEFT:
			if(snake.getDirection() != RIGHT) snake.turn(LEFT); break;
		case KEY_RIGHT:
			if(snake.getDirection() != LEFT) snake.turn(RIGHT); break;
		default:
			break;
	}
}
