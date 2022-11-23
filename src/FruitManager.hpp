#pragma once

#include <vector>
#include <random>

#include "Fruit.hpp"
#include "Vector2i.hpp"

class FruitManager {
	std::vector<Fruit> fruits;
	int min_fruits;

public:
	FruitManager(int min_fruits);

	const std::vector<Fruit>& getFruits() const;
	Fruit getFruit(int index) const;
	int getFruitCount() const;
	int getMinFruits() const;

	void add(Vector2i win_size);
	void remove(int index);
	void setMinFruits(int new_min);
};
