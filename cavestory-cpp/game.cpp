#include "game.h"
#include "input.h"
#include "SDL.h"
#include <algorithm>

/* Game class
* Holds all information for the main game loop
*/

/* Constants for frame calculations */
namespace {
    const int FPS = 50;
    const int MAX_FRAME_TIME = 1000 / FPS;
}

/* Constructor */
Game::Game() {
    SDL_Init(SDL_INIT_EVERYTHING);
    this->gameLoop();
}

/* Destructor */
Game::~Game() {

}

void Game::gameLoop() {
    Graphics graphics;
    Input input;
    SDL_Event event;

    this->_level = Level("map1", graphics);
    this->_player = Player(graphics, _level.getPlayerSpawnPoint());
    this->_hud = HUD(graphics, _player);

    int LAST_UPDATE_TIME = SDL_GetTicks();

    // Start the game loop
    while (true) {
        input.beginNewFrame();

        // Poll for event
        if (SDL_PollEvent(&event)) {

            // Check if a key is down
            if (event.type == SDL_KEYDOWN) {
                if (event.key.repeat == 0) {
                    input.keyDownEvent(event);
                }
            }
            // Check if a key has been released
            else if (event.type == SDL_KEYUP) {
                input.keyUpEvent(event);
            }
            // Check if exiting window
            else if (event.type == SDL_QUIT) {
                return;
            }
        }

        // Check if ESC was pressed to quit
        if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
            return;
        }
        // Check for other player input
        else if (input.isKeyHeld(SDL_SCANCODE_LEFT)) {
            _player.moveLeft();
        }
        else if (input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
            _player.moveRight();
        }

        if (input.isKeyHeld(SDL_SCANCODE_UP)) {
            _player.lookUp();
        }
        else if (input.isKeyHeld(SDL_SCANCODE_DOWN)) {
            _player.lookDown();
        }

        if (input.wasKeyReleased(SDL_SCANCODE_UP)) {
            _player.stopLookingUp();
        }
        if (input.wasKeyReleased(SDL_SCANCODE_DOWN)) {
            _player.stopLookingDown();
        }

        if (input.wasKeyPressed(SDL_SCANCODE_Z)) {
            _player.jump();
        }
        if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
            _player.stopMoving();
        }

        // Limiting frame time
        const int CURRENT_TIME_MS = SDL_GetTicks();
        int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

        _graphics = graphics;

        this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
        LAST_UPDATE_TIME = CURRENT_TIME_MS;

        this->draw(graphics);
    }
}

void Game::draw(Graphics& graphics) {
    graphics.clear();
    this->_level.draw(graphics);
    this->_player.draw(graphics);

    this->_hud.draw(graphics);

    graphics.flip();
}

void Game::update(float elapsedTime) {
    this->_player.update(elapsedTime);
    this->_level.update(elapsedTime, _player);
    this->_hud.update(elapsedTime, _player);

    // Check collisions
    std::vector<Rectangle> others;
    if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
        this->_player.handleTileCollisions(others);
    }

    std::vector<Slope> otherSlopes;
    if ((otherSlopes = _level.checkSlopeCollisions(_player.getBoundingBox())).size() > 0) {
        this->_player.handleSlopeCollisions(otherSlopes);
    }

    std::vector<Door> otherDoors;
    if ((otherDoors = _level.checkDoorCollisions(_player.getBoundingBox())).size() > 0) {
        this->_player.handleDoorCollision(otherDoors, _level, _graphics);
    }

    // Check enemies
    std::vector<Enemy*> otherEnemies;
    if ((otherEnemies = _level.checkEnemyCollisions(_player.getBoundingBox())).size() > 0) {
        _player.handleEnemyCollisions(otherEnemies);
    }
}