#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "globals.h"
#include "graphics.h"
#include "sprite.h"
#include <map>
#include <string>
#include <vector>

/* Animated Sprite class
* Holds logic for animating sprite
*/

class AnimatedSprite : public Sprite {

public:
	AnimatedSprite();
	AnimatedSprite(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height,
		float posX, float posY, float timeToUpdate);

	/* void playAnimation
	* Plays animation if it isn't already playing
	* Loops on default unless once is true
	*/
	void playAnimation(std::string animation, bool once = false);

	/* float update
	* Updates the animated sprite (timer)
	*/
	void update(float elapsedTime);

	/* void draw
	* Draws the sprite to the screen
	*/
	void draw(Graphics& graphics, int x, int y);


protected:
	double _timeToUpdate;
	bool _currentAnimationOnce;
	std::string _currentAnimation;

	/* void addAnimation
	* Adds an animation to the map of animations for the sprite
	*/
	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);

	/* void resetAnimations
	* Resets all animations associated with the sprite
	*/
	void resetAnimations();

	/* void stopAnimation
	* Stops the current animation
	*/
	void stopAnimation();

	/* void setVisible
	* Changes the visibility of the animated sprite
	*/
	void setVisible(bool visible);

	/* void animationDone
	* Logic that happens when an animation ends
	*/
	virtual void animationDone(std::string currentAnimation) = 0;

	/* void setUpAnimations
	* A required function that sets up all animations for a sprite
	*/
	virtual void setUpAnimations() = 0;


private:
	std::map<std::string, std::vector<SDL_Rect>> _animations;
	std::map<std::string, Vector2> _offsets;

	int _frameIndex;
	double _timeElapsed;
	bool _visible;

};

#endif // !ANIMATED_SPRITE_H
