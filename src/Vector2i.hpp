#pragma once

struct Vector2i {
	int x;
	int y;
};

bool inline operator==(const Vector2i& left, const Vector2i& right) {
	return left.x == right.x && left.y == right.y;
}

bool inline operator!=(const Vector2i& left, const Vector2i& right) {
	return !(left == right);
}
