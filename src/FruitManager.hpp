#ifndef FRUITMANAGER_HPP
#define FRUITMANAGER_HPP

#include <vector>
#include <random>

#include "Fruit.hpp"
#include "IntPair.hpp"

class FruitManager {
	std::vector<Fruit> fruits;

public:
	std::vector<Fruit> getFruits();
	Fruit getFruit(int index);
	int getFruitCount();

	void add(IntPair win_size);
	void remove(int index);
};

#endif
