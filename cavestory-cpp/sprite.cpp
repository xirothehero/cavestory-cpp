#include "sprite.h"
#include "graphics.h"
#include "globals.h"
#include <algorithm>

/* Sprite class
* Holds all information for individual sprites
*/

/* Constructors */
Sprite::Sprite() {}

Sprite::Sprite(Graphics& graphics, const std::string& filepath, int sourceX, int sourceY, int width, int height,
	float posX, float posY) : _x(posX), _y(posY) 
{
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.w = width;
	this->_sourceRect.h = height;

	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filepath));
	if (this->_spriteSheet == NULL) {
		printf("\nError: Unable to load image\n");
	}

	_boundingBox = Rectangle(_x, _y, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
}

/* Destructor */
Sprite::~Sprite() {}

void Sprite::draw(Graphics& graphics, int x, int y) {
	SDL_Rect destinationRect = { 
		x, 
		y, 
		this->_sourceRect.w * globals::SPRITE_SCALE, 
		this->_sourceRect.h * globals::SPRITE_SCALE 
	};
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRect);
}

void Sprite::update() {
	_boundingBox = Rectangle(_x, _y, _sourceRect.w * globals::SPRITE_SCALE, _sourceRect.h * globals::SPRITE_SCALE);
}

const Rectangle Sprite::getBoundingBox() const { return _boundingBox; }

const sides::Side Sprite::getCollisionSide(Rectangle& other) const {
	int amtRight, amtLeft, amtTop, amtBottom;
	amtRight = abs(getBoundingBox().getRight() - other.getLeft());
	amtLeft = abs(other.getRight() - getBoundingBox().getLeft());
	amtTop = abs(other.getBottom() - getBoundingBox().getTop());
	amtBottom = abs(getBoundingBox().getBottom() - other.getTop());

	// Tomer Tzadok, Ep10
	int lowest = std::min({ amtRight, amtLeft, amtTop, amtBottom });

	return
		lowest == amtRight ? sides::RIGHT :
		lowest == amtLeft ? sides::LEFT :
		lowest == amtTop ? sides::TOP :
		lowest == amtBottom ? sides::BOTTOM :
		sides::NONE;
}

void Sprite::setSourceRectX(int value) {
	_sourceRect.x = value;
}

void Sprite::setSourceRectY(int value) {
	_sourceRect.y = value;
}

void Sprite::setSourceRectW(int value) {
	_sourceRect.w = value;
}

void Sprite::setSourceRectH(int value) {
	_sourceRect.h = value;
}