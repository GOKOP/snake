#ifndef MENU_OPTION_HPP
#define MENU_OPTION_HPP

#include <string>
#include "Enums.hpp"

class MenuOption {
public:
	std::string name;
	GameState target_state;

	MenuOption(std::string new_name, GameState new_target_state);
};

#endif
