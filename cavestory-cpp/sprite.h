#ifndef SPRITE_H
#define SPRITE_H

/* Sprite class
* Holds all information for individual sprites
*/

#include "globals.h"
#include "rectangle.h"
#include "SDL.h"
#include <string>

struct Graphics;

class Sprite {

public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, 
		float posX, float posY);
	virtual ~Sprite();

	/* void update
	* Update bounding box of sprite
	* To be overridden by child classes
	*/
	virtual void update();

	/* void draw
	* Draw sprite to window
	*/
	void draw(Graphics &graphics, int x, int y);

	/* Rectangle getBoundingBox
	* Get bounding box of sprite
	*/
	const Rectangle getBoundingBox() const;

	/* Side getCollisionSide
	* Calculate where other Rectangle collides with sprite's bounding box
	*/
	const sides::Side getCollisionSide(Rectangle& other) const;

	/* float getX
	* Get X position of sprite in window
	*/
	const inline float getX() const { return _x; }

	/* float getY
	* Get Y position of sprite in window
	*/
	const inline float getY() const { return _y; }

	/* void setSourceRectX
	* Set X position of Rectangle being pulled from spritesheet
	*/
	void setSourceRectX(int value);

	/* void setSourceRectY
	* Set Y position of Rectangle being pulled from spritesheet
	*/
	void setSourceRectY(int value);

	/* void setSourceRectW
	* Set width of Rectangle being pulled from spritesheet
	*/
	void setSourceRectW(int value);

	/* void setSourceRectH
	* Set height of Rectangle being pulled from spritesheet
	*/
	void setSourceRectH(int value);


protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;
	float _x, _y;

	Rectangle _boundingBox;

};

#endif // !SPRITE_H
