#pragma once

#include <SDL.h>

#include <array>
#include <string>
#include <vector>

class Game {
public:
    Game();
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    bool initialize(const std::string& title, int width, int height);
    void run();

private:
    void processEvents(bool& running);
    void update(float deltaSeconds);
    void render();
    void shutdown();

    SDL_Window* window_;
    SDL_Renderer* renderer_;
    int width_;
    int height_;
};
