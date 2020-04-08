#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"
#include <map>

/* Input class
* Keeps track of keyboard state
*/

class Input {

public:

	/* void beginNewFrame
	*
	* This gets called at the beginning of each new frame
	* to reset the keys that are no longer relevant
	*/
	void beginNewFrame();

	/* void keyDownEvent
	*
	* This gets called when a key has been pressed
	*/
	void keyUpEvent(const SDL_Event& event);

	/* void keyUpEvent
	*
	* This gets called when a key has been released
	*/
	void keyDownEvent(const SDL_Event& event);

	/* bool wasKeyPressed
	*
	* Checks if a certain key was pressed during the current frame
	*/
	bool wasKeyPressed(SDL_Scancode key);

	/* bool wasKeyReleased
	*
	* Checks if a certain key was released during the current frame
	*/
	bool wasKeyReleased(SDL_Scancode key);

	/* bool isKeyHeld
	*
	* Checks if a certain key is currently being held
	*/
	bool isKeyHeld(SDL_Scancode key);
	
private:
	std::map<SDL_Scancode, bool> _heldKeys;
	std::map<SDL_Scancode, bool> _pressedKeys;
	std::map<SDL_Scancode, bool> _releasedKeys;

};

#endif // !INPUT_H
