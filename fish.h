#pragma once
#include "settings.h"
class Fish {
public:
	enum FishType { BLUE, GREEN, RED, PURPLE };
	Fish(FishType type);
	void update();
	void draw(sf::RenderWindow& window);
	void spawn();
	sf::Vector2f getPosition();
	sf::FloatRect getHitBox() { return sprite.getGlobalBounds(); }
	FishType getType() { return type; }
	float fishSpeedX = -2.f;
	void speedChange1();
	void speedChange2();
private:
	sf::Texture texture;
	sf::Sprite sprite;
	FishType type;
};
void Fish::spawn() {
	float fishHeight = sprite.getLocalBounds().height;
	float x = rand() % (int)(WINDOW_WIDTH)+WINDOW_WIDTH;
	float y = rand() % (int)(WINDOW_HEIGHT - fishHeight);
	sprite.setPosition(x, y);
}
Fish::Fish(FishType type) {
	this->type = type;
	switch (type) {
	case BLUE:
		texture.loadFromFile(BLUE_FILENAME);
		break;
	case GREEN:
		texture.loadFromFile(GREEN_FILENAME);
		break;
	case RED:
		texture.loadFromFile(RED_FILENAME);
		break;
	case PURPLE:
		texture.loadFromFile(PURPLE_FILENAME);
		break;
	}
	sprite.setTexture(texture);
	spawn();
}

void Fish::speedChange1() {
	fishSpeedX = -3.f;
}
void Fish::speedChange2() {
	fishSpeedX = -4.f;
}

void Fish::update() {
	sprite.move(fishSpeedX, 0.f);
	sf::Vector2f pos = sprite.getPosition();
	float fishWidht = sprite.getLocalBounds().width;
	if (pos.x < 0.f - fishWidht)
	{
		spawn();
	}
}

void Fish::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}
sf::Vector2f Fish::getPosition() { return sprite.getPosition(); }