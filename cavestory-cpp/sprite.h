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
	virtual void update();
	void draw(Graphics &graphics, int x, int y);

	const Rectangle getBoundingBox() const;
	const sides::Side getCollisionSide(Rectangle& other) const;

	const inline float getX() const { return _x; }
	const inline float getY() const { return _y; }

	void setSourceRectX(int value);
	void setSourceRectY(int value);
	void setSourceRectW(int value);
	void setSourceRectH(int value);


protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;
	float _x, _y;

	Rectangle _boundingBox;

private:

};

#endif // !SPRITE_H
