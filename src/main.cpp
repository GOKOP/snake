#include <curses.h>
#include <unistd.h>
#include <cstdlib>

#include "IntPair.hpp"
#include "Snake.hpp"

#define SNAKE_HEAD '@'
#define SNAKE_DEAD 'X'
#define SNAKE_BODY 'O'
#define BORDER '#'

enum GameState {
	RUNNING,
	LOST
};

void millisleep(int millisec);
WINDOW* curses_init(bool first, IntPair win_size);
IntPair find_centered_pos(IntPair win_size, IntPair term_size);
void print_char(WINDOW* win, IntPair pos, chtype ch);
void print_win(WINDOW* win, Snake snake, GameState state);
GameState advanceGame(Snake &snake, IntPair win_size);
void processInput(WINDOW* win, Snake &snake);

int main() {
	IntPair win_size(40,20);
	WINDOW* win = curses_init(true, win_size);
	Snake snake(IntPair(20,10), RIGHT);
	GameState state = RUNNING;
	
	while(true) {
		processInput(win, snake);
		if(state != LOST)
			state = advanceGame(snake, win_size);
		print_win(win, snake, state);
		millisleep(10);
	}

	endwin();
	return 0;
}

void millisleep(int millisec) {
	usleep(millisec*10000);
}

WINDOW* curses_init(bool first, IntPair win_size) {
	if(first==true) {
		initscr();
		cbreak();
		noecho();
		curs_set(0);
	}
	
	IntPair term_size; 
	getmaxyx(stdscr, term_size.y, term_size.x);

	IntPair win_pos = find_centered_pos(win_size, term_size);
	WINDOW* win = newwin(win_size.y, win_size.x, win_pos.y, win_pos.x);
	nodelay(win, TRUE);
	keypad(win, TRUE);

	return win;
}

IntPair find_centered_pos(IntPair win_size, IntPair term_size) {
	IntPair centered_pos;
	centered_pos.x = term_size.x/2 - win_size.x/2;
	centered_pos.y = term_size.y/2 - win_size.y/2;

	return centered_pos;
}

void print_char(WINDOW* win, IntPair pos, chtype ch) {
	IntPair max_pos;
	getmaxyx(win, max_pos.y, max_pos.x);

	if(pos.x>=0 && pos.x<max_pos.x && pos.y>=0 && pos.y<max_pos.y) {
		wmove(win, pos.y, pos.x);
		waddch(win, ch);
	}
}

void print_win(WINDOW* win, Snake snake, GameState state) {
	werase(win);
	wborder(win, BORDER, BORDER, BORDER, BORDER, BORDER, BORDER, BORDER, BORDER);
	
	for(int i=0; i<snake.getBodySize(); ++i) {
		if(i==0)
			print_char(win, snake.getBodyPiecePos(i), SNAKE_HEAD);
		else
			print_char(win, snake.getBodyPiecePos(i), SNAKE_BODY);
	}

	if(state == LOST)
		print_char(win, snake.getBodyPiecePos(0), SNAKE_DEAD);

	wrefresh(win);
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
