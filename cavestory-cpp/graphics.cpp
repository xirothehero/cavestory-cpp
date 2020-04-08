#include "globals.h"
#include "graphics.h"
#include "SDL.h"
#include "SDL_image.h"

/* Graphics class
* Holds all information dealing with graphics for the game
*/

/* Constructor */
Graphics::Graphics() {
    SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, 
        &this->_window, &this->_renderer);              // Can access class variables with "this->"
    SDL_SetWindowTitle(_window, "CaveStory");           // Or just refer to them directly
}

/* Destructor */
Graphics::~Graphics() {
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
}

SDL_Surface* Graphics::loadImage(const std::string& filePath) {

    if (_spriteSheets.count(filePath) == 0) {
        _spriteSheets[filePath] = IMG_Load(filePath.c_str());
    }
    return _spriteSheets[filePath];

}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
    SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

void Graphics::flip() {
    SDL_RenderPresent(this->_renderer);
}

void Graphics::clear() {
    SDL_RenderClear(this->_renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
    return _renderer;
}
