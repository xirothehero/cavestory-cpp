#include "animatedtile.h"
#include "graphics.h"
#include "SDL.h"

AnimatedTile::AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, SDL_Texture* tileset, Vector2 size, Vector2 pos) :
	Tile(tileset, size, tilesetPositions.at(0), pos), _tilesetPositions(tilesetPositions), _duration(duration), _tileToDraw(0) {}

void AnimatedTile::update(int elapsedTime) {
	// Timer code
	if (_amountOfTime <= 0) {
		if (_tileToDraw == _tilesetPositions.size() - 1) {
			_tileToDraw = 0;
		}
		else {
			_tileToDraw++;
		}
		_amountOfTime = _duration;
	}
	else {
		_amountOfTime -= elapsedTime;
	}

	Tile::update(elapsedTime);
}

void AnimatedTile::draw(Graphics& graphics) {
	SDL_Rect destRect = { _position.x, _position.y, _size.x * globals::SPRITE_SCALE, _size.y * globals::SPRITE_SCALE };
	SDL_Rect sourceRect = {
		_tilesetPositions.at(_tileToDraw).x, _tilesetPositions.at(_tileToDraw).y, _size.x, _size.y
	};
	graphics.blitSurface(_tileset, &sourceRect, &destRect);
}