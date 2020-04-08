#include "globals.h"
#include "graphics.h"
#include "level.h"
#include "player.h"
#include "SDL.h"
#include "tinyxml2.h"
#include "utils.h"

#include <algorithm>
#include <cmath>
#include <sstream>

/* Level class
* Manages map generation and keeps track of entities within level
*/

using namespace tinyxml2;

/* Constructors */
Level::Level() {}

Level::Level(std::string mapName, Graphics& graphics) :
	_mapName(mapName), _size(Vector2(0,0)) 
{
	loadMap(_mapName, graphics);
}

/* Destructor */
Level::~Level() {}


std::vector<Rectangle> Level::checkTileCollisions(const Rectangle& other) {
	std::vector<Rectangle> others;
	for (int i = 0; i < _collisionRects.size(); i++) {
		if (_collisionRects.at(i).collidesWith(other)) {
			others.push_back(_collisionRects.at(i));
		}
	}
	return others;
}

std::vector<Slope> Level::checkSlopeCollisions(const Rectangle& other) {
	std::vector<Slope> others;
	for (int i = 0; i < _slopes.size(); i++) {
		if (_slopes.at(i).collidesWith(other)) {
			others.push_back(_slopes.at(i));
		}
	}
	return others;
}

std::vector<Door> Level::checkDoorCollisions(const Rectangle& other) {
	std::vector<Door> others;
	for (int i = 0; i < _doorList.size(); i++) {
		if (_doorList.at(i).collidesWith(other)) {
			others.push_back(_doorList.at(i));
		}
	}
	return others;
}

std::vector<Enemy*> Level::checkEnemyCollisions(const Rectangle& other) {
	std::vector<Enemy*> others;
	for (int i = 0; i < _enemies.size(); i++) {
		Rectangle bb = _enemies.at(i)->getBoundingBox();
		if (bb.collidesWith(other)) {
			others.push_back(_enemies.at(i));
		}
	}
	return others;
}

const Vector2 Level::getPlayerSpawnPoint() const {
	return _spawnPoint;
}

void Level::loadMap(std::string mapName, Graphics& graphics) {
	// Parse the .tmx file
	XMLDocument doc;
	std::stringstream ss;
	ss << "maps/" << mapName << ".tmx";
	doc.LoadFile(ss.str().c_str());

	XMLElement* mapNode = doc.FirstChildElement("map");

	// Get the width and height of the whole map and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	_size = Vector2(width, height);

	// Get the width and height of the tiles and store it in _tilesize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	_tileSize = Vector2(tileWidth, tileHeight);

	// Load the tilesets
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL) {
		while (pTileset) {
			int firstgid;
			const char* source = pTileset->Attribute("source");

			char* path;
			std::stringstream ss;
			std::stringstream tsSs;
			// Fix from ashton woollard's comment in Ep9
			for (int i = 3; i < (int)strlen(source) - 4; i++) {
				ss << source[i];
				tsSs << source[i];
			}
			ss << ".png";
			tsSs << ".tsx";
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			_tilesets.push_back(Tileset(tex, firstgid));

			// Get all animations for tileset
			XMLDocument tilesetDoc; 
			tilesetDoc.LoadFile(tsSs.str().c_str());
			
			XMLElement* pTilesetA = tilesetDoc.FirstChildElement("tileset");

			XMLElement* pTileA = pTilesetA->FirstChildElement("tile");
			if (pTileA != NULL) {
				while (pTileA) {
					AnimatedTileInfo ati;
					ati.startTileId = pTileA->IntAttribute("id") + firstgid;
					ati.tilesetFirstGid = firstgid;

					XMLElement* pAnim = pTileA->FirstChildElement("animation");
					if (pAnim != NULL) {
						while (pAnim) {
							XMLElement* pFrame = pAnim->FirstChildElement("frame");
							if (pFrame != NULL) {
								while (pFrame) {
									ati.tileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
									ati.duration = pFrame->IntAttribute("duration");

									pFrame = pFrame->NextSiblingElement("frame");
								}
							}
							
							pAnim = pAnim->NextSiblingElement("animation");
						}
					}
					_animatedtileInfos.push_back(ati);


					pTileA = pTileA->NextSiblingElement("tile");
				}
			}


			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	// Loading the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) {
			// Loading the data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					// Loading the tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
						int tileCounter = 0;
						while (pTile) {
							// Build each individual tile here

							//If no gid, no tile
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}
							
							// Get the tileset for this specific gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							int closest = 0;
							for (int i = 0; i < _tilesets.size(); i++) {
								if (_tilesets[i].firstGid <= gid) {
									if (_tilesets[i].firstGid > closest) {
										closest = _tilesets[i].firstGid;
										tls = _tilesets.at(i);
									}
								}
							}

							if (tls.firstGid == -1) {
								// No proper tileset found
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							// Figure out position of tile in level
							int xx = 0;
							int yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);
							Vector2 finalTilePos = Vector2(xx, yy);

							// Calc position of tile in tileset
							Vector2 finalTilesetPos = getTilesetPosition(tls, gid, tileWidth, tileHeight);

							// Build the actual tile and add it to the level's tile list
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (int i = 0; i < _animatedtileInfos.size(); i++) {
								if (_animatedtileInfos.at(i).startTileId == gid) {
									ati = _animatedtileInfos.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							if (isAnimatedTile) {
								std::vector<Vector2> tilesetPositions;
								for (int i = 0; i < ati.tileIds.size(); i++) {
									tilesetPositions.push_back(getTilesetPosition(
										tls, ati.tileIds.at(i), tileWidth, tileHeight
									));
								}
								AnimatedTile animTile(tilesetPositions, ati.duration,
									tls.texture, Vector2(tileWidth, tileHeight), finalTilePos);
								_animatedTileList.push_back(animTile);
							}
							else {
								Tile tile(tls.texture, Vector2(tileWidth, tileHeight), finalTilesetPos, finalTilePos);
								_tileList.push_back(tile);
							}
							tileCounter++;

							pTile = pTile->NextSiblingElement("tile");
						}
					}

					pData = pData->NextSiblingElement("data");
				}
			}

			pLayer = pLayer->NextSiblingElement("layer");
		}
	}

	// Parse out collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "Collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, w, h;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						w = pObject->FloatAttribute("width");
						h = pObject->FloatAttribute("height");
						_collisionRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(w) * globals::SPRITE_SCALE,
							std::ceil(h) * globals::SPRITE_SCALE
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			// Other object groups, TBD
			else if (ss.str() == "SpawnPoints") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss2;
						ss2 << name;
						if (ss2.str() == "Player" || "player") {
							this->_spawnPoint = Vector2(
								std::ceil(x) * globals::SPRITE_SCALE, 
								std::ceil(y) * globals::SPRITE_SCALE
							);
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "Slopes") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						std::vector<Vector2> points;
						Vector2 p1 =
							Vector2(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y")));

						XMLElement* pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL) {
							std::vector<std::string> pairs;
							const char* pointString = pPolyline->Attribute("points");

							std::stringstream plss;
							plss << pointString;
							Utils::split(plss.str(), pairs, ' ');

							for (int i = 0; i < pairs.size(); i++) {
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
							}
						}

						for (int i = 0; i < points.size(); i += 2) {
							_slopes.push_back(Slope(
								Vector2((p1.x + points.at(i < 2 ? i : i - 1).x) * globals::SPRITE_SCALE,
										(p1.y + points.at(i < 2 ? i : i - 1).y) * globals::SPRITE_SCALE),
								Vector2((p1.x + points.at(i < 2 ? i + 1 : i).x) * globals::SPRITE_SCALE,
										(p1.y + points.at(i < 2 ? i + 1 : i).y) * globals::SPRITE_SCALE)
							));
						}

						/* Alt _slopes.push_back() loop by Minh, Ep11

						for (int i = 0; i < points.size() - 1; i++) {
							Vector2 point1 = Vector2((p1.x + points.at(i).x) * globals::SPRITE_SCALE,
													 (p1.y + points.at(i).y) * globals::SPRITE_SCALE);
							Vector2 point2 = Vector2((p1.x + points.at(i + 1).x) * globals::SPRITE_SCALE,
													 (p1.y + points.at(i + 1).y) * globals::SPRITE_SCALE);
							Slope slope = Slope(point1, point2);

							_slopes.push_back(slope);
						}
						*/

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "Doors") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						float w = pObject->FloatAttribute("width");
						float h = pObject->FloatAttribute("height");
						Rectangle rect = Rectangle(x, y, w, h);

						XMLElement* pProperties = pObject->FirstChildElement("properties");
						if (pProperties != NULL) {
							while (pProperties) {
								XMLElement* pProperty = pProperties->FirstChildElement("property");
								if (pProperty != NULL) {
									while (pProperty) {
										const char* name = pProperty->Attribute("name");
										std::stringstream nss;
										nss << name;
										if (nss.str() == "Destination") {
											const char* value = pProperty->Attribute("value");
											std::stringstream mss;
											mss << value;
											Door door = Door(rect, mss.str());
											_doorList.push_back(door);
										}

										pProperty = pProperty->NextSiblingElement("property");
									}
								}

								pProperties = pProperties->NextSiblingElement("properties");
							}
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "Enemies") {
			float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ess;
						ess << name;
						if (ess.str() == "Bat" || "bat") {
							_enemies.push_back(new Bat(graphics,
									Vector2(std::floor(x) * globals::SPRITE_SCALE, std::floor(y) * globals::SPRITE_SCALE)
								));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}

Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.texture, NULL, NULL, &tilesetWidth, &tilesetHeight);

	int tsxx = (gid - 1) % (tilesetWidth / tileWidth); // Fix by Shawn Hayes, Ep12
	tsxx *= tileWidth;
	// Fix from rawrwoww, Ep9
	tsxx = (tsxx < 0) ? 15 * tileWidth : tsxx;

	int tsyy = 0;
	int amt = (gid - tls.firstGid) / (tilesetWidth / tileWidth);
	tsyy = tileHeight * amt;

	Vector2 finalTilesetPos = Vector2(tsxx, tsyy);
	return finalTilesetPos;
}

void Level::update(int elapsedTime, Player& player) {
	for (int i = 0; i < _animatedTileList.size(); i++) {
		_animatedTileList.at(i).update(elapsedTime);
	}
	for (int i = 0; i < _enemies.size(); i++) {
		_enemies.at(i)->update(elapsedTime, player);
	}
}

void Level::draw(Graphics& graphics) {
	for (int i = 0; i < _tileList.size(); i++) {
		_tileList.at(i).draw(graphics);
	}
	for (int i = 0; i < _animatedTileList.size(); i++) {
		_animatedTileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->draw(graphics);
	}
}

