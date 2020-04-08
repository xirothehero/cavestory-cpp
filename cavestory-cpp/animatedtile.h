#ifndef ANIMATED_TILE_H
#define ANIMATED_TILE_H

#include "globals.h"
#include "tile.h"
#include <vector>

/* AnimatedTile class
* Holds information on AnimatedTile from Tiled
*/

class AnimatedTile : public Tile {

public:
	AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, SDL_Texture* tileset, Vector2 size, Vector2 pos);

	/* void update
	* Updates state of animated tile
	*/
	void update(int elapsedTime);

	/* void draw
	* Draws animated tile to window
	*/
	void draw(Graphics& graphics);

protected:
	int _amountOfTime = 0;
	bool _notDone = false;

private:
	std::vector<Vector2> _tilesetPositions;
	int _tileToDraw;
	int _duration;

};

/* Info required to construct an AnimatedTile */
struct AnimatedTileInfo {
public:
	int tilesetFirstGid;
	int startTileId;
	std::vector<int> tileIds;
	int duration;
};

#endif // !ANIMATED_TILE_H
