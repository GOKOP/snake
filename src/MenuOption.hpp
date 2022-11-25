#pragma once

#include <string>
#include "enums.hpp"

struct MenuOption {
	std::string name;
	GameState target_state;
};
