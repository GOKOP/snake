#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <vector>
#include "BodyPiece.hpp"

class Snake {
	std::vector<BodyPiece> body; //head is the first element
	bool fed;

public:
	Snake(IntPair start_pos, Direction start_direction);

	int get_body_size();
	IntPair get_body_piece_pos(int index);
	bool is_fed();

	void feed();
	void advance_pos();
	void turn(Direction new_direction);
};

#endif
