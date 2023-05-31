#pragma once
#include <vector>
#include "settings.h"
#include "player.h"
#include "okean.h"
#include "fish.h"
#include "textObj.h"
#include "bonus.h"
class Game {
public:
	enum GameState { INSTRUCTIONS, PLAY, GAME_OVER, YOU_WIN, RESULTS };
	GameState gameState = INSTRUCTIONS;
	Game() : text(" ", TEXT_POS, FONT_SIZE1, sf::Color::Red), text1(" ", TEXT_POS1, FONT_SIZE1,sf::Color::Green),
	instruction(" ", TEXT_POS_INSTRUCTIONS, FONT_SIZE1,sf::Color::White),
	results(" ", TEXT_POS_RESULTS, FONT_SIZE1,sf::Color::White){
		window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
		window.setFramerateLimit(FPS);
		Fish* fish1 = new Fish(Fish::FishType::RED);
		Fish* fish2 = new Fish(Fish::FishType::GREEN);
		Fish* fish3 = new Fish(Fish::FishType::BLUE);
		Fish* fish4 = new Fish(Fish::FishType::RED);
		Fish* fish5 = new Fish(Fish::FishType::GREEN);
		Fish* fish6 = new Fish(Fish::FishType::BLUE);
		Fish* fish7 = new Fish(Fish::FishType::PURPLE);
		fishSprites.push_back(fish1);
		fishSprites.push_back(fish2);
		fishSprites.push_back(fish3);
		fishSprites.push_back(fish4);
		fishSprites.push_back(fish5);
		fishSprites.push_back(fish6);
		fishSprites.push_back(fish7);
	}

	void play() {
		while (window.isOpen()) {
			checkEvents();
			update();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))  gameState = PLAY; 
			if (player.getHp() <= 0) gameState = GAME_OVER;
			if (player.getPoint() >= 250) gameState = YOU_WIN;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) gameState = RESULTS;
			draw();
		}
	}

private:
	sf::RenderWindow window;
	Player player;
	Okean ocean;
	std::list<Fish*> fishSprites;
	Textobj text;
	Textobj text1;
	std::list<Bonus*> bonusSprites;
	Textobj instruction;
	Textobj results;

	void checkEvents() {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	void update() {
		switch (gameState) {
		case INSTRUCTIONS:
			instruction.update("YOU NEED TO GET 250 POINTS TO WIN\nYOU SHOULD NOT SHOOT PURPLE FISH\n        <<W>> GO UP   <<S>> GO DOWN\n               <<B>> BLUE LASER\n               <<N>> GREEN LASER\n               <<M>> RED LASER\n            PRESS <<A>> TO PLAY ");
			break;
		case PLAY:
			player.update();
			for (auto& fish : fishSprites) {
				fish->update();
			}
			for (auto& bonus : bonusSprites) {
				bonus->update();
			}
			ocean.update();
			checkCollisions();
			if (player.getHp() > 100) {
				player.startHp();
			}
			if (player.getPoint() == 100 ) {
				for (auto& fish : fishSprites) {
					fish->speedChange1();
				}
			}
			if (player.getPoint() == 200) {
				for (auto& fish : fishSprites) {
					fish->speedChange2();
				}
			}
			break;
		case GAME_OVER:
			text.update("                 GAME OVER\n PRESS <<Q>> TO CHECK RESULTS");
			break;
		case YOU_WIN:
			text1.update("                  YOU WIN\n PRESS <<Q>> TO CHECK RESULTS");
			player.startPoint();
			break;
		case RESULTS:
			results.update("MISSED FISH: " + std::to_string(player.getMissedFish())+"\n" "FINAL HP: "+ std::to_string(player.getHp()));
			break;
		}
	}

	void fishCollideLaser();
	void fishCollideBound();

	void checkCollisions() {
		fishCollideLaser();
		fishCollideBound();
	}

	void draw() {
		switch (gameState) {
		case INSTRUCTIONS:
			window.clear();
			ocean.draw(window);
			instruction.draw(window);
			window.display();
			break;
		case PLAY:
			window.clear();
			ocean.draw(window);
			player.draw(window);
			for (const auto& fish : fishSprites) {
				fish->draw(window);
			}
			for (const auto& bonus : bonusSprites) {
				bonus->draw(window);
			}
			window.display();
			break;
		case GAME_OVER:
			window.clear();
			ocean.draw(window);
			text.draw(window);
			window.display();
			break;
		case YOU_WIN:
			window.clear();
			ocean.draw(window);
			text1.draw(window);
			window.display();
			break;
		case RESULTS:
			window.clear();
			ocean.draw(window);
			results.draw(window);
			window.display();
			break;
		}
	}
};
void Game::fishCollideLaser() {
	auto laserSprites = player.getLaserSprites();
	for (auto& fish : fishSprites) {
		sf::FloatRect fishBounds = fish->getHitBox();
		for (auto& laser : (*laserSprites)) {
			sf::FloatRect laserBounds = laser->getHitBox();
			if (fishBounds.intersects(laserBounds)) {
				if ((fish->getType() == Fish::FishType::BLUE && laser->getType() == Laser::LaserType::BLUE) ||
					(fish->getType() == Fish::FishType::RED && laser->getType() == Laser::LaserType::RED) ||
					(fish->getType() == Fish::FishType::GREEN && laser->getType() == Laser::LaserType::GREEN))
				{
					size_t chance = rand() % 10001;
					if (chance < 1500) {
						size_t bonusType = rand() % Bonus::BonusType::TYPES_QTY;
						Bonus* bonus = new Bonus((Bonus::BonusType)bonusType, fish->getPosition());
						bonusSprites.push_back(bonus);
					}
					fish->spawn();
					laser->setHit();
					player.pointChange();
				}
				if (fish->getType() == Fish::FishType::PURPLE) {
					fish->spawn(); 
					laser->setHit();
					player.pointChange1();
				}
			}
			if (laser->getPosX() > WINDOW_WIDTH) {
				laser->setHit();
			}
		}

	}
	(*laserSprites).remove_if([](Laser* laser) {return laser->isHit(); });
	for (auto& bonus : bonusSprites) {
		sf::FloatRect playerBounds = player.getHitBox();
		sf::FloatRect bonusBounds = bonus->getHitBox();
		if (bonusBounds.intersects(playerBounds)) {
			bonus->act(player);
			bonus->setDel();
		}
		if (bonus->getPosX() < 0.f - bonusBounds.width) {
			bonus->setDel();
		}
	}
	bonusSprites.remove_if([](Bonus* bonus) {return bonus->isToDel(); });
}

void Game::fishCollideBound() {
	for (auto& fish : fishSprites) {
		sf::FloatRect fishBounds = fish->getHitBox();
		if ((fish->getType() == Fish::FishType::BLUE && fish->getPosition().x <= 1.f - fishBounds.width) ||
			(fish->getType() == Fish::FishType::RED && fish->getPosition().x <= 1.f - fishBounds.width) ||
			(fish->getType() == Fish::FishType::GREEN && fish->getPosition().x <= 1.f - fishBounds.width)) {
			player.hpChange();
			player.missedFishChange();
		}

		if (player.shieldIsActive()) {
			sf::FloatRect shieldBounds = player.getShieldHitBox();
			if (fishBounds.intersects(shieldBounds)) {
				if (fish->getType() == Fish::FishType::BLUE ||
					fish->getType() == Fish::FishType::RED ||
					fish->getType() == Fish::FishType::GREEN)
				{
					fish->spawn();
					player.pointChange();
				}
				if (fish->getType() == Fish::FishType::PURPLE) {
					fish->spawn();
					player.pointChange1();
				}
			}
		}
	}
}