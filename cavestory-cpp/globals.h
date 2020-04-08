#ifndef GLOBALS_H
#define GLOBALS_H

/* Constants for screen size and sprite scaling */
namespace globals {
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	const float SPRITE_SCALE = 2.0f;
}

/* Sides for rectangles/bounding boxes for collision calculations */
namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};

	/* Side getOppositeSide
	* Get side opposite of side passed in
	*/
	const inline Side getOppositeSide(Side side) {
		return
			side == TOP ? BOTTOM :
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}

/* Movement and orientation directions */
enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

/* 2D Vector for storing (x, y) pairs */
struct Vector2 {
	int x, y;
	
	Vector2() : x(0), y(0) {}
	Vector2(int x, int y) : x(x), y(y) {}
	static Vector2 zero() {
		return Vector2(0, 0);
	}

	Vector2 operator*(int multiplier) {
		return Vector2(this->x * multiplier, this->y * multiplier);
	}

};

#endif // !GLOBALS_H
