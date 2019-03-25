#ifndef INTPAIR_HPP
#define INTPAIR_HPP

class IntPair {
public:
	int x;
	int y;

	IntPair();
	IntPair(int new_x, int new_y);
	bool operator==(IntPair other);
};

#endif

