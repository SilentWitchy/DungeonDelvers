#pragma once

#include <SDL.h>
#include <string>

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

    struct Button {
        SDL_Rect bounds;
        std::string label;
    };

    void handleMainMenuEvent(const SDL_Event& event, bool& running);
    void renderMainMenu();
    void drawButton(const Button& button, bool hovered);
    void drawText(const std::string& text, int x, int y, int scale, SDL_Color color);
    const std::array<std::string, 7>& glyphForChar(char c) const;

    SDL_Window* window_;
    SDL_Renderer* renderer_;
    int width_;
    int height_;
    std::vector<Button> mainMenuButtons_;
};
