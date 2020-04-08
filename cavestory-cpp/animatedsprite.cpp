#include "animatedsprite.h"
#include "sprite.h"
#include "SDL.h"

/* Animated Sprite class
* Animates the sprite
*/

AnimatedSprite::AnimatedSprite() {}

AnimatedSprite::AnimatedSprite(Graphics& graphics, const std::string& filepath, int sourceX, int sourceY, int width, int height,
	float posX, float posY, float timeToUpdate) : 
	Sprite(graphics, filepath, sourceX, sourceY, width, height, posX, posY),
	_frameIndex(0), _timeToUpdate(timeToUpdate), _visible(true),
	_currentAnimationOnce(false), _currentAnimation(""), _timeElapsed(0) {}


void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset) {
	std::vector<SDL_Rect> rectangles;
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { x + i * width, y, width, height };
		rectangles.push_back(newRect);
	}
	this->_animations.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, rectangles));
	this->_offsets.insert(std::pair<std::string, Vector2>(name, offset));
}

void AnimatedSprite::resetAnimations() {
	this->_animations.clear();
	this->_offsets.clear();
}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
	this->_currentAnimationOnce = once;
	if (this->_currentAnimation != animation) {
		this->_currentAnimation = animation;
		this->_frameIndex = 0;
	}
}

void AnimatedSprite::setVisible(bool visible) {
	_visible = visible;
}

void AnimatedSprite::stopAnimation() {
	this->_frameIndex = 0;
	this->animationDone(this->_currentAnimation);
}

void AnimatedSprite::update(float elapsedTime) {
	Sprite::update();

	this->_timeElapsed += elapsedTime;
	if (this->_timeElapsed > this->_timeToUpdate) {
		this->_timeElapsed -= this->_timeToUpdate;
		if (this->_frameIndex < this->_animations[_currentAnimation].size() - 1) {
			this->_frameIndex++;
		}
		else {
			if (_currentAnimationOnce == true) {
				setVisible(false);
			}
			stopAnimation();
		}
	}
}

void AnimatedSprite::draw(Graphics &graphics, int x, int y) {
	if (this->_visible) {
		SDL_Rect destinationRect;
		destinationRect.x = x + _offsets[_currentAnimation].x;
		destinationRect.y = y + _offsets[_currentAnimation].y;
		destinationRect.w = this->_sourceRect.w * globals::SPRITE_SCALE;
		destinationRect.h = this->_sourceRect.h * globals::SPRITE_SCALE;

		SDL_Rect sourceRect = this->_animations[_currentAnimation][_frameIndex];
		graphics.blitSurface(_spriteSheet, &sourceRect, &destinationRect);
	}
}