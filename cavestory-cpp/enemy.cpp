#include "animatedsprite.h"
#include "enemy.h"

/* Enemy class
* Holds information for enemy entity
*/

/* Constructors */
Enemy::Enemy() {}

Enemy::Enemy(Graphics& graphics, const std::string filepath, int srcX, int srcY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filepath, srcX, srcY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(LEFT), _maxHealth(0), _currentHealth(0) {}


void Enemy::update(int elapsedTime, Player& player) {
	AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics& graphics) {
	AnimatedSprite::draw(graphics, _x, _y);
}


/* Bat class
* Holds information for a bat, a specific type of enemy
*/

/* Constructors */
Bat::Bat() {}

Bat::Bat(Graphics& graphics, Vector2 spawnPoint) : 
	Enemy(graphics, "sprites/NpcCemet.png", 32, 32, 16, 16, spawnPoint, 140),
	_startingX(spawnPoint.x), _startingY(spawnPoint.y), _shouldMoveUp(false) {
	
	this->setUpAnimations();
	this->playAnimation("FlyLeft");
}


void Bat::update(int elapsedTime, Player& player) {
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	this->playAnimation(this->_direction == RIGHT ? "FlyRight" : "FlyLeft");

	// Move up or down
	this->_y += _shouldMoveUp ? -0.02 : 0.02;
	if (this->_y > (_startingY + 20) || this->_y < (_startingY - 20)) {
		_shouldMoveUp = !_shouldMoveUp;
	}

	Enemy::update(elapsedTime, player);
}

void Bat::draw(Graphics& graphics) {
	Enemy::draw(graphics);
}

void Bat::animationDone(std::string currentAnimation) {
	
}

void Bat::setUpAnimations() {
	this->addAnimation(3, 32, 32, "FlyLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 32, 48, "FlyRight", 16, 16, Vector2(0, 0));
}

void Bat::touchPlayer(Player* player) {
	player->gainHealth(-1);
}
