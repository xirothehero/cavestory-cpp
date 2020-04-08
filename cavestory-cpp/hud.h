#ifndef HUD_H
#define HUD_H

#include "player.h"
#include "sprite.h"

struct Graphics;

class HUD {

public:
	HUD();
	HUD(Graphics& graphics, Player& player);
	void update(int elapsedTime, Player& player);
	void draw(Graphics& graphics);

private:
	Player _player;
	
	// Health
	Sprite _healthBarSprite;
	Sprite _healthNumber1;
	Sprite _currentHealthBar;

	// EXP
	Sprite _lvlWord;
	Sprite _lvlNum;
	Sprite _expBar;

	// Weapon info
	Sprite _slash;
	Sprite _dashes;

};


#endif // !HUD_H