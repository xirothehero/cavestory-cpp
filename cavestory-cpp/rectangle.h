#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "globals.h"

/* Rectangle class
* Holds information for a rectangle object
* Used for collisions
*/

class Rectangle {

public:
	Rectangle() {}
	Rectangle(int x, int y, int width, int height) :
		_x(x), _y(y), _width(width), _height(height) {}

	/* int getCenterX
	* Returns the X coordinate of the center of the rectangle
	*/
	const int getCenterX() const { return _x + _width / 2; }

	/* int getCenterY
	* Returns the Y coordinate of the center of the rectangle
	*/
	const int getCenterY() const { return _y + _height / 2; }

	/* int getLeft
	* Returns the position of the left side of the rectangle along the X axis
	*/
	const int getLeft() const { return _x; }

	/* int getRight
	* Returns the position of the right side of the rectangle along the X axis
	*/
	const int getRight() const { return _x + _width; }

	/* int getTop
	* Returns the position of the top side of the rectangle along the Y axis
	*/
	const int getTop() const { return _y; }

	/* int getBottom
	* Returns the position of the bottom side of the rectangle along the Y axis
	*/
	const int getBottom() const { return _y + _height; }

	/* int getWidth
	* Returns the width of the rectangle
	*/
	const int getWidth() const { return _width; }

	/* int getHeight
	* Returns the height of the rectangle
	*/
	const int getHeight() const { return _height; }

	/* int getSide
	* Return the positional value of the inputted side
	*/
	const int getSide(sides::Side side) const {
		return
			side == sides::LEFT ? getLeft() :
			side == sides::RIGHT ? getRight() :
			side == sides::TOP ? getTop() :
			side == sides::BOTTOM ? getBottom() :
			side == sides::NONE;
	}

	/* const bool collidesWith
	* Takes in another Rectangle and checks if the two are colliding
	*/
	const bool collidesWith(const Rectangle& other) {
		return
			this->getRight() >= other.getLeft() &&
			this->getLeft() <= other.getRight() &&
			this->getTop() <= other.getBottom() &&
			this->getBottom() >= other.getTop();
	}

	/* bool isValidRectangle
	* Checks if this rectangle has non-negative parameters
	*/
	const bool isValidRectangle() const {
		return (_x >= 0 && _y >= 0 && _width >= 0 && _height >= 0);
	}

	/* Rectangle getRect
	* Returns this Rectangle object
	*/
	const inline Rectangle getRect() const { return *this; }

private:
	int _x, _y, _width, _height;
};

#endif // !RECTANGLE_H
