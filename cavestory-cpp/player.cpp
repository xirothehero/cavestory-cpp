#include "enemy.h"
#include "graphics.h"
#include "level.h"
#include "player.h"

/* Player class
* Holds information and logic for the player
*/

/* Constants for player movement */
namespace player_constants {
	const float WALK_SPEED = 0.2f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;

	const float JUMP_SPEED = 0.7f;
}

/* Constructors */
Player::Player() {}

Player::Player(Graphics& graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "sprites/MyChar.png", 0, 0, 16, 16, float(spawnPoint.x), float(spawnPoint.y), 100.0f),
	_dx(0), _dy(0), _facing(Direction::RIGHT), _grounded(false), _lookingUp(false), _lookingDown(false),
	_maxHealth(3), _currentHealth(3)
{
	graphics.loadImage("sprites/MyChar.png");
	this->setUpAnimations();
	this->playAnimation("IdleRight");
}


void Player::setUpAnimations() {
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 48, 0, "IdleLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 48, 16, "IdleRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 48, 0, "RunLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 48, 16, "RunRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 96, 0, "LookDownLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 96, 16, "LookDownRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 112, 0, "LookBackwardsLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 112, 16, "LookBackwardsRight", 16, 16, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnim) {}

const float Player::getX() const {
	return _x;
}

const float Player::getY() const {
	return _y;
}

void Player::moveLeft() {
	if (_lookingDown && _grounded) {
		return;
	}
	_dx = -player_constants::WALK_SPEED;
	if (!_lookingUp) {
		playAnimation("RunLeft");
	}
	_facing = Direction::LEFT;
}

void Player::moveRight() {
	if (_lookingDown && _grounded) {
		return;
	}
	_dx = player_constants::WALK_SPEED;
	if (!_lookingUp) {
		playAnimation("RunRight");
	}
	_facing = Direction::RIGHT;
}

void Player::stopMoving() {
	_dx = 0.0f;
	if (!_lookingUp && !_lookingDown) {
		playAnimation(_facing == Direction::RIGHT ? "IdleRight" : "IdleLeft");
	}
}

void Player::lookUp() {
	_lookingUp = true;
	if (_dx == 0) {
		playAnimation(_facing == Direction::RIGHT ? "IdleRightUp" : "IdleLeftUp");
	}
	else {
		playAnimation(_facing == Direction::RIGHT ? "RunRightUp" : "RunLeftUp");
	}
}

void Player::stopLookingUp() {
	_lookingUp = false;
}

void Player::lookDown() {
	_lookingDown = true;
	if (_grounded) {
		// We need to interact, so turn backwards
		playAnimation(_facing == Direction::RIGHT ? "LookBackwardsRight" : "LookBackwardsLeft");
	}
	else {
		playAnimation(_facing == Direction::RIGHT ? "LookDownRight" : "LookDownLeft");
	}
}

void Player::stopLookingDown() {
	_lookingDown = false;
}

void Player::jump() {
	if (_grounded) {
		_dy = 0;
		_dy -= player_constants::JUMP_SPEED;
	}
	_grounded = false;
}

void Player::handleTileCollisions(std::vector<Rectangle>& others) {
	// Figure out which side the collision occurred on and move the player accordingly
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_dy = 0;
				this->_y = float(others.at(i).getBottom() + 1);
				if (_grounded) {
					_dx = 0;
					_x -= (_facing == Direction::RIGHT ? 1.0f : -1.0f);
				}
				break;
			case sides::BOTTOM:
				this->_y = float(others.at(i).getTop() - this->getBoundingBox().getHeight() - 1);
				this->_dy = 0;
				this->_grounded = true;
				break;
			case sides::LEFT:
				this->_x = float(others.at(i).getRight() + 1);
				break;
			case sides::RIGHT:
				this->_x = float(others.at(i).getLeft() - this->getBoundingBox().getWidth() - 1);
				break;
			}
		}
	}
}

void Player::handleSlopeCollisions(std::vector<Slope>& others) {
	for (int i = 0; i < others.size(); i++) {
		// Calculate where on the slope the player's bottom center is touching
		// and use y=mx+b to figure out the y position to place the player at

		// First, calculate y-int (b) using one of the points -> b = y - mx
		int b = others.at(i).getP1().y - (others.at(i).getSlope() * fabs(others.at(i).getP1().x));

		// Get player's centerX
		int centerX = _boundingBox.getCenterX();

		// y = (m * centerX) + b
		int y = others.at(i).getSlope() * centerX + b - 8; // 8 is a temp fix

		//Reposition player to correct y
		if (_grounded) {
			_y = y - _boundingBox.getHeight();
			_grounded = true;
		}

	}
}

void Player::handleDoorCollision(std::vector<Door>& others, Level& level, Graphics& graphics) {
	// Check if the player is grounded and holding DOWN, while colliding with a door rect
	for (int i = 0; i < others.size(); i++) {
		if (_grounded && _lookingDown) {
			// Need to free Enemies
			level = Level(others.at(i).getDestination(), graphics);
			_x = level.getPlayerSpawnPoint().x;
			_y = level.getPlayerSpawnPoint().y;
			break;
		}
	}
}

void Player::handleEnemyCollisions(std::vector<Enemy*>& others) {
	for (int i = 0; i < others.size(); i++) {
		others.at(i)->touchPlayer(this);
	}
}

void Player::gainHealth(int amount) {
	_currentHealth += amount;
	if (_currentHealth < 0) {
		_currentHealth = 0;
	}
	if (_currentHealth > _maxHealth) {
		_currentHealth = _maxHealth;
	}
}

void Player::update(float elapsedTime) {
	// Apply gravity
	if (_dy <= player_constants::GRAVITY_CAP) {
		_dy += player_constants::GRAVITY * elapsedTime;
	}

	// Update x and y
	_x += (_dx * elapsedTime);
	_y += (_dy * elapsedTime);
	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics& graphics) {
	AnimatedSprite::draw(graphics, _x, _y);
}

