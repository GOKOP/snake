#include "BodyPiece.hpp"

BodyPiece::BodyPiece(IntPair new_pos, Direction new_direction) {
	pos 	  = new_pos;
	direction = new_direction;
}

IntPair BodyPiece::get_pos() {
	return pos;
}

Direction BodyPiece::get_direction() {
	return direction;
}

void BodyPiece::move(Direction new_direction) {
	switch(direction) {
		case UP:
			pos = IntPair(pos.x, pos.y-1); break;
		case DOWN:
			pos = IntPair(pos.x, pos.y+1); break;
		case LEFT:
			pos = IntPair(pos.x-1, pos.y); break;
		case RIGHT:
			pos = IntPair(pos.x+1, pos.y); break;
		default:
			break;
	}
	direction = new_direction;
}

void BodyPiece::turn(Direction new_direction) {
	direction = new_direction;
}
