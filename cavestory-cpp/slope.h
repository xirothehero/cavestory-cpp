#ifndef SLOPE_H
#define SLOPE_H

#include "globals.h"
#include "rectangle.h"
#include <cmath>

/* Slope class
* Represents a slope in the map
* Used for collision calculations
*/

class Slope {

public:
	Slope() {}
	Slope(Vector2 p1, Vector2 p2) : _p1(p1), _p2(p2) {
		// Calculate slope
		if (_p2.x - _p1.x != 0) {
			_slope = (fabs(_p2.y) - fabs(_p1.y)) / (fabs(_p2.x) - fabs(_p1.x));
		}
	}

	/* Vector2 getP1
	* Returns first point of slope line
	*/
	const inline Vector2 getP1() const { return _p1; }

	/* Vector2 getP2
	* Returns second point of slope line
	*/
	const inline Vector2 getP2() const { return _p2; }

	/* float getSlope
	* Returns slope of line
	*/
	const inline float getSlope() const { return _slope; }

	/* bool collidesWith
	* Determines collision with Rectangle parameter
	*/
	const bool collidesWith(const Rectangle& other) {
		return
			(other.getRight() >= _p2.x && other.getLeft() <= _p1.x && other.getTop() <= _p2.y && other.getBottom() >= _p1.y) ||
			(other.getRight() >= _p1.x && other.getLeft() <= _p2.x && other.getTop() <= _p1.y && other.getBottom() >= _p2.y) ||
			(other.getRight() >= _p2.x && other.getLeft() <= _p1.x && other.getTop() <= _p1.y && other.getBottom() >= _p2.y) ||
			(other.getRight() >= _p1.x && other.getLeft() <= _p2.x && other.getTop() <= _p2.y && other.getBottom() >= _p1.y);
	}

private:
	Vector2 _p1, _p2;
	float _slope;
};

#endif // !SLOPE_H
