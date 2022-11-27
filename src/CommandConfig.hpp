#pragma once

#include <string_view>
#include <optional>
#include "Vector2i.hpp"

class CommandConfig {
	Vector2i window_size;
	int snake_delay;
	int min_fruits;
	int beg_length;
	bool is_ok;

	std::optional<int> stringToPositiveInt(std::string_view str) const;
	std::optional<Vector2i> readWinSize(std::string_view size_str) const;
	std::optional<int> readSnakeDelay(std::string_view delay_str) const;

public:

	CommandConfig(int argc, char* argv[]);

	Vector2i getWindowSize() const;
	int getSnakeDelay() const;
	int getMinFruits() const;
	int getBegLength() const;
	bool isOk() const;
};
