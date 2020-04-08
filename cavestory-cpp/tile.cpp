#include "tile.h"
#include "graphics.h"
#include "SDL.h"

Tile::Tile() {}

Tile::Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position) :
	_tileset(tileset), _size(size), _tilesetPosition(tilesetPosition),
	_position(Vector2(position.x * globals::SPRITE_SCALE, position.y * globals::SPRITE_SCALE)) {}

void Tile::update(int elapsedTime) {}

void Tile::draw(Graphics& graphics) {
	SDL_Rect destRect = { _position.x, _position.y, _size.x * globals::SPRITE_SCALE, _size.y * globals::SPRITE_SCALE };
	SDL_Rect sourceRect = { _tilesetPosition.x, _tilesetPosition.y, _size.x, _size.y };
	graphics.blitSurface(_tileset, &sourceRect, &destRect);
}