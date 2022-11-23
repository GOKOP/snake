#pragma once

#include <vector>
#include "MenuOption.hpp"

class Menu {
	std::string header;
	std::string corner_text;
	std::vector<MenuOption> options;
	int selected_index;

public:
	
	Menu(std::string_view header, std::string_view corner_text);

	std::string_view getHeader() const;
	std::string_view getCornerText() const;
	int getOptionCount() const;
	int getSelection() const;
	const MenuOption& getOption(int index) const;

	void addOption(const MenuOption& option);
	void moveSelection(Direction dir);
};
