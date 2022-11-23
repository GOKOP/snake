#include "Menu.hpp"

Menu::Menu(std::string_view header, std::string_view corner_text):
	header(header),
	corner_text(corner_text),
	selected_index(0)
{}

std::string Menu::getHeader() {
	return header;
}

std::string Menu::getCornerText() {
	return corner_text;
}

int Menu::getOptionCount() {
	return options.size();
}

int Menu::getSelection() {
	return selected_index;
}

MenuOption Menu::getOption(int index) {
	return options[index];
}

void Menu::addOption(MenuOption option) {
	options.push_back(option);
}

void Menu::moveSelection(Direction dir) {
	if(dir==RIGHT || dir==LEFT) return;

	if(dir==UP)        --selected_index;
	else if(dir==DOWN) ++selected_index;

	if(selected_index < 0)
		selected_index = options.size()-1;
	else if(selected_index >= options.size())
		selected_index = 0;
}
