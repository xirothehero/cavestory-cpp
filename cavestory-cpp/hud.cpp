#include "graphics.h"
#include "hud.h"

/* HUD class
* Holds information on game heads-up display
*/

/* Constructors */
HUD::HUD() {}

HUD::HUD(Graphics& graphics, Player& player) {
	_player = player;
	_healthBarSprite = Sprite(graphics, "sprites/TextBox.png", 0, 40, 64, 8, 35, 70);
	_healthNumber1 = Sprite(graphics, "sprites/TextBox.png", 0, 56, 8, 8, 66, 70);
	_currentHealthBar = Sprite(graphics, "sprites/TextBox.png", 0, 25, 39, 5, 83, 72);
	_lvlWord = Sprite(graphics, "sprites/TextBox.png", 81, 81, 11, 7, 38, 55);
	_lvlNum = Sprite(graphics, "sprites/TextBox.png", 0, 56, 8, 8, 66, 52);
	_expBar = Sprite(graphics, "sprites/TextBox.png", 0, 72, 40, 8, 83, 52);
	_slash = Sprite(graphics, "sprites/TextBox.png", 72, 48, 8, 8, 100, 36);
	_dashes = Sprite(graphics, "sprites/TextBox.png", 81, 51, 15, 11, 132, 26);
}


void HUD::update(int elapsedTime, Player& player) {
	_player = player;
	_healthNumber1.setSourceRectX(8 * _player.getCurrentHealth());
	
	float healthPercent = (float)_player.getCurrentHealth() / _player.getMaxHealth();
	_currentHealthBar.setSourceRectW(std::floor(39 * healthPercent));
}

void HUD::draw(Graphics& graphics) {
	_healthBarSprite.draw(graphics, _healthBarSprite.getX(), _healthBarSprite.getY());
	_healthNumber1.draw(graphics, _healthNumber1.getX(), _healthNumber1.getY());
	_currentHealthBar.draw(graphics, _currentHealthBar.getX(), _currentHealthBar.getY());
	_lvlWord.draw(graphics, _lvlWord.getX(), _lvlWord.getY());
	_lvlNum.draw(graphics, _lvlNum.getX(), _lvlNum.getY());
	_expBar.draw(graphics, _expBar.getX(), _expBar.getY());
	_slash.draw(graphics, _slash.getX(), _slash.getY());
	_dashes.draw(graphics, _dashes.getX(), _dashes.getY()); 
}