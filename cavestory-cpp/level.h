#ifndef LEVEL_H
#define LEVEL_H

#include "animatedtile.h"
#include "door.h"
#include "enemy.h"
#include "globals.h"
#include "rectangle.h"
#include "slope.h"
#include "tile.h"
#include <string>
#include <vector>

struct Graphics;
struct Player;
struct SDL_Rect;
struct SDL_Texture;
struct Tileset;

class Level {

public:
	Level();
	Level(std::string mapName, Graphics& graphics);
	~Level();

	void update(int elapsedTime, Player& player);
	void draw(Graphics& graphics);

	std::vector<Rectangle> checkTileCollisions(const Rectangle& other);
	std::vector<Slope> checkSlopeCollisions(const Rectangle& other);
	std::vector<Door> checkDoorCollisions(const Rectangle& other);
	std::vector<Enemy*> checkEnemyCollisions(const Rectangle& other);

	const Vector2 getPlayerSpawnPoint() const;

private:
	std::string _mapName;
	Vector2 _spawnPoint;

	Vector2 _size;
	Vector2 _tileSize;

	SDL_Texture* _backgroundTexture;
	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;
	std::vector<Rectangle> _collisionRects;
	std::vector<Slope> _slopes;

	std::vector<AnimatedTile> _animatedTileList;
	std::vector<AnimatedTileInfo> _animatedtileInfos;

	std::vector<Door> _doorList;
	std::vector<Enemy*> _enemies;

	/* void loadMap
	* Loads a map
	*/
	void loadMap(std::string mapName, Graphics& graphics);
	Vector2 getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight);
};

// Tileset struct
struct Tileset {
	SDL_Texture* texture;
	int firstGid;

	Tileset() {
		this->firstGid = -1;
	}

	Tileset(SDL_Texture* texture, int firstGid) {
		this->texture = texture;
		this->firstGid = firstGid;
	}
};

#endif // !LEVEL_H
