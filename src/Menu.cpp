#include "Menu.hpp"

Menu::Menu(std::string_view header, std::string_view corner_text):
	header(header),
	corner_text(corner_text),
	selected_index(0)
{}

std::string_view Menu::getHeader() const {
	return header;
}

std::string_view Menu::getCornerText() const {
	return corner_text;
}

int Menu::getOptionCount() const {
	return options.size();
}

int Menu::getSelection() const {
	return selected_index;
}

const MenuOption& Menu::getOption(int index) const {
	return options[index];
}

void Menu::addOption(const MenuOption& option) {
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
