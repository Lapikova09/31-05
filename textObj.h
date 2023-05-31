#pragma once
#include <SFML/Graphics.hpp>
#include "settings.h"
class Textobj {
private:
	sf::Text text;
	sf::Font font;
public:
	Textobj(std::string str, sf::Vector2f position, int fontSize, sf::Color color) {
		font.loadFromFile(FONT_FILENAME);
		text.setFont(font);
		text.setString(str);
		text.setFillColor(color);
		text.setCharacterSize(fontSize);
		text.setPosition(position);
	}
	void update(std::string str) {
		text.setString(str);
	}
	void draw(sf::RenderWindow& window) {
		window.draw(text);
	}
};