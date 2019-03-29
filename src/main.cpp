#include <curses.h>
#include <unistd.h>
#include <cstdlib>

#include "IntPair.hpp"
#include "Snake.hpp"
#include "Display.hpp"

enum GameState {
	RUNNING,
	LOST
};

void millisleep(int millisec);
GameState advanceGame(Snake &snake, IntPair win_size);
void processInput(WINDOW* win, Snake &snake);

int main() {
	IntPair win_size(40,20);
	Display display(win_size);
	Snake snake(IntPair(20,10), RIGHT);
	GameState state = RUNNING;
	
	while(true) {
		processInput(display.getWindow(), snake);

		if(state != LOST)
			state = advanceGame(snake, win_size);
		
		display.printWin(snake);
		
		if(state == LOST)
			display.printDead(snake.getBodyPiecePos(0));

		millisleep(10);
	}

	endwin();
	return 0;
}

void millisleep(int millisec) {
	usleep(millisec*10000);
}

GameState advanceGame(Snake &snake, IntPair win_size) {
	snake.advancePos();	
	
	if(snake.getBodyPiecePos(0).x==0 || snake.getBodyPiecePos(0).y==0 ||
	   snake.getBodyPiecePos(0).x==win_size.x-1 || snake.getBodyPiecePos(0).y==win_size.y-1)
		return LOST;
	else {
		for(int i=1; i<snake.getBodySize(); ++i) {
			if(snake.getBodyPiecePos(0) == snake.getBodyPiecePos(i))
				return LOST;
		}
	}

	return RUNNING;
}

void processInput(WINDOW* win, Snake &snake) {
	switch(wgetch(win)) {
		case KEY_UP:
			snake.turn(UP); break;
		case KEY_DOWN:
			snake.turn(DOWN); break;
		case KEY_LEFT:
			snake.turn(LEFT); break;
		case KEY_RIGHT:
			snake.turn(RIGHT); break;
		default:
			break;
	}
}
