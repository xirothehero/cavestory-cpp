#ifndef TILE_H
#define TILE_H

#include "globals.h"

/* Tile class
* Holds information for a tile from Tiled map
*/

struct SDL_Texture;
struct Graphics;

class Tile {

public:
	Tile();
	Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position);

	/* void update
	* Updates state of tile
	* Note: only really used with AnimatedTiles
	*/
	void update(int elapsedTime);

	/* void draw
	* Draws tile to window
	*/
	void draw(Graphics& graphics);

protected:
	SDL_Texture* _tileset;
	Vector2 _size;
	Vector2 _tilesetPosition;
	Vector2 _position;

};

#endif // !TILE_H
