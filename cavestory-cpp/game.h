#ifndef GAME_H
#define GAME_H

/* Game class
* Holds all information for the main game loop
*/

#include "graphics.h"
#include "hud.h"
#include "level.h"
#include "player.h"

class Game {

public:
	Game();
	~Game();

private:

	/* void gameLoop
	* Initialize graphics and input and sprite
	* Polls for Event
	*/
	void gameLoop();

	/* void draw
	* Draw all level, player, and HUD sprites to the window
	*/
	void draw(Graphics &graphics);

	/* void update
	* Update level, player, and HUD
	* Check all collisions
	*/
	void update(float elapsedTime);

	/* Fields */
	Player _player;
	Level _level;
	HUD _hud;
	Graphics _graphics;
};

#endif // !GAME_H