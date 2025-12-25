#include "game/Game.hpp"
#include <SDL.h>
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    {
        Game game;
        if (!game.initialize("Dungeon Delvers", 1280, 720)) {
            SDL_Quit();
            return EXIT_FAILURE;
        }

        game.run();
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}
