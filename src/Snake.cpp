#include "Snake.hpp"

Snake::Snake(IntPair start_pos, Direction start_direction) {
	body.push_back(BodyPiece(start_pos, start_direction));
	fed = true;
}

int Snake::get_body_size() {
	return body.size();
}

IntPair Snake::get_body_piece_pos(int index) {
	return body[index].get_pos();	
}

bool Snake::is_fed() {
	return fed;
}

void Snake::feed() {
	fed = true;
}

void Snake::advance_pos() {
	for(int i=body.size()-1; i>=0; --i) {
		if(i==body.size()-1 && fed) {
			IntPair old_pos         = body[i].get_pos();
			Direction old_direction = body[i].get_direction();
			body.push_back(BodyPiece(old_pos, old_direction));
			fed = false;
		}

		if(i==0)
			body[i].move(body[i].get_direction());
		else
			body[i].move(body[i-1].get_direction());
	}
}

void Snake::turn(Direction new_direction) {
	body[0].turn(new_direction);	
}
