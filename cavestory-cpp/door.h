#ifndef DOOR_H
#define DOOR_H

#include "globals.h"
#include "rectangle.h"
#include <string>

/* Door class
* Holds information on Door from Tiled
* Main method to go to a new map
*/

class Door : public Rectangle {
	
public:
	Door() {}
	Door(Rectangle rect, std::string dest) :
		Rectangle(rect.getLeft()* globals::SPRITE_SCALE, rect.getTop()* globals::SPRITE_SCALE,
			rect.getWidth()* globals::SPRITE_SCALE, rect.getHeight()* globals::SPRITE_SCALE),
		_destination(dest) {}

	/* Rectangle getRectangle
	* Returns Rectangle which represents collider
	*/
	const inline Rectangle getRectangle() const { return this->getRect(); }

	/* string getDestination
	* Returns destination name for loading map
	*/
	const inline std::string getDestination() const { return _destination; }
	
private:
	std::string _destination;

};

#endif // !DOOR_H
