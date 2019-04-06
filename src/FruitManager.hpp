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

	void add(IntPair win_size, std::mt19937& gen);
	void remove(int index);
};

#endif
