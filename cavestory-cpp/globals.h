#ifndef GLOBALS_H
#define GLOBALS_H

namespace globals {
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	const float SPRITE_SCALE = 2.0f;
}

namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};

	const inline Side getOppositeSide(Side side) {
		return
			side == TOP ? BOTTOM :
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}

enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

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
