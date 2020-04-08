#ifndef ENEMY_H
#define ENEMY_H

#include "animatedsprite.h"
#include "globals.h"
#include "player.h"
#include <string>

struct Graphics;

/* Enemy class
* Holds information for enemy entity
*/

class Enemy : public AnimatedSprite {

public:
	Enemy();
	Enemy(Graphics& graphics, const std::string filepath, int srcX, int srcY, int width, int height, Vector2 spawnPoint, int timeToUpdate);

	/* void update
	* Updates state of enemy
	*/
	virtual void update(int elapsedTime, Player& player);
	
	/* void draw
	* Draws enemy to window
	*/
	virtual void draw(Graphics& graphics);

	/* void touchPlayer
	* Logic when enemy touches player
	*/
	virtual void touchPlayer(Player* player) = 0;

	/* int getMaxHealth
	* Returns enemy's maximum health
	*/
	const inline int getMaxHealth() const { return _maxHealth; }

	/* int getCurrentHealth
	* Returns enemy's current health
	*/
	const inline int getCurrentHealth() const { return _currentHealth; }

protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;

};


/* Bat class
* Holds information for a bat, a specific type of enemy
*/

class Bat : public Enemy {

public:
	Bat();
	Bat(Graphics& graphics, Vector2 spawnPoint);

	/* void update
	* Updates state of bat
	*/
	void update(int elapsedTime, Player& player);

	/* void draw
	* Draws bat to window
	*/
	void draw(Graphics& graphics);

	/* void animationDone
	* Logic that happens when an animation ends
	*/
	void animationDone(std::string currentAnimation);

	/* void setUpAnimations
	* A required function that sets up all animations for a sprite
	*/
	void setUpAnimations();

	/* void touchPlayer
	* Logic when enemy touches player
	*/
	void touchPlayer(Player* player);

private:
	float _startingX, _startingY;
	bool _shouldMoveUp;

};

#endif // !ENEMY_H