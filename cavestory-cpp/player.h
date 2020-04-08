#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "door.h"
#include "globals.h"
#include "slope.h"

struct Enemy;
struct Graphics;
struct Level;

class Player : public AnimatedSprite {

public:
	Player();
	Player(Graphics& graphics, Vector2 spawnPoint);
	void draw(Graphics& graphics);
	void update(float elapsedTime);

	/* void moveLeft
	* Moves the player left by -dx
	*/
	void moveLeft();

	/* void moveRight
	* Moves the player right by dx
	*/
	void moveRight();

	/* void stopMoving
	* Stops moving the player starts idle
	*/
	void stopMoving();

	/* void lookUp
	* The player looks up
	*/
	void lookUp();

	/* void stopLookingUp
	* The player stops looking up
	*/
	void stopLookingUp();

	/* void lookDown
	* The player looks down
	*/
	void lookDown();

	/* void stopLookingDown
	* The player stops looking down
	*/
	void stopLookingDown();

	/* void jump
	* Starts jumping
	*/
	void jump();

	virtual void animationDone(std::string currentAnim);
	virtual void setUpAnimations();

	/* void handleTileCollisions
	* Handles collisions with ALL tiles the player is colliding with
	*/
	void handleTileCollisions(std::vector<Rectangle>& others);

	/* void handleSlopeCollisions
	* Handles collisions with ALL slopes the player is colliding with
	*/
	void handleSlopeCollisions(std::vector<Slope>& others);

	/* void handleDoorCollision
	* Handles collision with ONE door the player is colliding with
	*/
	void handleDoorCollision(std::vector<Door>& others, Level& level, Graphics& graphics);

	/* void handleEnemyCollisions
	* Handles collisions with ALL enemies the player is colliding with
	*/
	void handleEnemyCollisions(std::vector<Enemy*>& others);

	const float getX() const;
	const float getY() const;

	const inline int getMaxHealth() const { return _maxHealth; }
	const inline int getCurrentHealth() const { return _currentHealth; }

	void gainHealth(int amount);

private:
	float _dx, _dy;
	Direction _facing;
	bool _grounded;
	bool _lookingUp;
	bool _lookingDown;

	int _maxHealth;
	int _currentHealth;
};

#endif // !PLAYER_H

