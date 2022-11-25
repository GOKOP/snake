#include "Snake.hpp"

Snake::Snake(Vector2i start_pos, Direction start_direction, int size):
	body({start_pos}),
	how_fed(size),
	head_direction(start_direction)
{}

int Snake::getBodySize() const {
	return body.size();
}

Vector2i Snake::getBodyPiecePos(int index) const {
	return body[index];
}

Vector2i Snake::getHeadPos() const {
	return body[0];
}

Direction Snake::getDirection() const {
	return head_direction;
}

int Snake::howFed() const {
	return how_fed;
}

void Snake::feed(int amount) {
	how_fed += amount;
}

void Snake::advancePos() {
	for(int i=body.size()-1; i>=0; --i) {
		if(i==body.size()-1 && how_fed>0) {
			body.push_back(body[i]);
			how_fed -= 1;
		}

		if(i==0)
			move_head();
		else
			body[i] = body[i-1];
	}
}

void Snake::turn(Direction new_direction) {
	head_direction = new_direction;
}

void Snake::move_head() {
	switch(head_direction) {
		case Direction::Up:
			body[0] = {body[0].x, body[0].y-1}; break;
		case Direction::Down:
			body[0] = {body[0].x, body[0].y+1}; break;
		case Direction::Left:
			body[0] = {body[0].x-1, body[0].y}; break;
		case Direction::Right:
			body[0] = {body[0].x+1, body[0].y}; break;
		default:
			break;
	}
}

