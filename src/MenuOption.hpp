#pragma once

#include <string>
#include "Enums.hpp"

struct MenuOption {
	std::string name;
	GameState target_state;
};
