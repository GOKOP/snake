#ifndef FRUITMANAGER_HPP
#define FRUITMANAGER_HPP

#include <vector>
#include <random>

#include "Fruit.hpp"
#include "IntPair.hpp"

class FruitManager {
	std::vector<Fruit> fruits;
	int min_fruits;

public:
	FruitManager(int new_min);
	std::vector<Fruit> getFruits();
	Fruit getFruit(int index);
	int getFruitCount();
	int getMinFruits();

	void add(IntPair win_size);
	void remove(int index);
	void setMinFruits(int new_min);
};

#endif
