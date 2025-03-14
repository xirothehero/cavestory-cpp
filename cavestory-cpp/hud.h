#ifndef HUD_H
#define HUD_H

#include "player.h"
#include "sprite.h"

struct Graphics;

/* HUD class
* Holds information on game heads-up display
*/

class HUD {

public:
	HUD();
	HUD(Graphics& graphics, Player& player);

	/* void update
	* Updates state of HUD elements
	*/
	void update(int elapsedTime, Player& player);

	/* void draw
	* Draws HUD elements to window
	*/
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