#include "game/Game.hpp"

#include <SDL.h>
#include <chrono>
#include <iostream>
#include <cctype>
#include <unordered_map>

namespace {
constexpr int TARGET_FPS = 60;
constexpr float TARGET_FRAME_TIME = 1.0f / static_cast<float>(TARGET_FPS);
using namespace std::string_literals;

using Glyph = std::array<std::string, 7>;

const std::unordered_map<char, Glyph> FONT = {
    {'A', {"01110", "10001", "10001", "11111", "10001", "10001", "10001"}},
    {'B', {"11110", "10001", "11110", "10001", "10001", "10001", "11110"}},
    {'C', {"01110", "10001", "10000", "10000", "10000", "10001", "01110"}},
    {'D', {"11110", "10001", "10001", "10001", "10001", "10001", "11110"}},
    {'E', {"11111", "10000", "11110", "10000", "10000", "10000", "11111"}},
    {'F', {"11111", "10000", "11110", "10000", "10000", "10000", "10000"}},
    {'G', {"01110", "10001", "10000", "10111", "10001", "10001", "01110"}},
    {'H', {"10001", "10001", "11111", "10001", "10001", "10001", "10001"}},
    {'I', {"01110", "00100", "00100", "00100", "00100", "00100", "01110"}},
    {'J', {"00111", "00010", "00010", "00010", "10010", "10010", "01100"}},
    {'K', {"10001", "10010", "11100", "10010", "10010", "10001", "10001"}},
    {'L', {"10000", "10000", "10000", "10000", "10000", "10000", "11111"}},
    {'M', {"10001", "11011", "10101", "10001", "10001", "10001", "10001"}},
    {'N', {"10001", "11001", "10101", "10011", "10001", "10001", "10001"}},
    {'O', {"01110", "10001", "10001", "10001", "10001", "10001", "01110"}},
    {'P', {"11110", "10001", "10001", "11110", "10000", "10000", "10000"}},
    {'Q', {"01110", "10001", "10001", "10001", "10101", "10010", "01101"}},
    {'R', {"11110", "10001", "10001", "11110", "10010", "10001", "10001"}},
    {'S', {"01111", "10000", "01110", "00001", "00001", "10001", "01110"}},
    {'T', {"11111", "00100", "00100", "00100", "00100", "00100", "00100"}},
    {'U', {"10001", "10001", "10001", "10001", "10001", "10001", "01110"}},
    {'V', {"10001", "10001", "10001", "10001", "10001", "01010", "00100"}},
    {'W', {"10001", "10001", "10001", "10101", "10101", "10101", "01010"}},
    {'X', {"10001", "10001", "01010", "00100", "01010", "10001", "10001"}},
    {'Y', {"10001", "10001", "01010", "00100", "00100", "00100", "00100"}},
    {'Z', {"11111", "00001", "00010", "00100", "01000", "10000", "11111"}},
    {' ', {"00000", "00000", "00000", "00000", "00000", "00000", "00000"}},
};
}

Game::Game()
    : window_(nullptr),
      renderer_(nullptr),
      width_(0),
      height_(0) {}

Game::~Game() { shutdown(); }

bool Game::initialize(const std::string& title, int width, int height) {
    width_ = width;
    height_ = height;

    window_ = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width_,
        height_,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window_) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << "\n";
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
        std::cerr << "Failed to create SDL renderer: " << SDL_GetError() << "\n";
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

    // Centered buttons sized relative to window for the main menu layout.
    const int buttonWidth = 300;
    const int buttonHeight = 60;
    const int verticalOffset = 120;
    const int spacing = 20;
    const int centerX = width_ / 2 - buttonWidth / 2;
    const int startY = height_ / 2 - buttonHeight / 2 + verticalOffset;

    mainMenuButtons_.push_back({SDL_Rect{centerX, startY, buttonWidth, buttonHeight}, "CREATE NEW WORLD"});
    mainMenuButtons_.push_back({SDL_Rect{centerX, startY + buttonHeight + spacing, buttonWidth, buttonHeight}, "QUIT"});
    return true;
}

void Game::run() {
    bool running = true;
    auto lastTick = std::chrono::steady_clock::now();

    while (running) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> delta = now - lastTick;
        lastTick = now;

        processEvents(running);
        update(delta.count());
        render();

        // Simple frame cap for early prototype builds.
        auto elapsed = std::chrono::steady_clock::now() - lastTick;
        std::chrono::duration<float> targetTime(TARGET_FRAME_TIME);
        if (elapsed < targetTime) {
            auto sleepDuration = targetTime - elapsed;
            SDL_Delay(static_cast<Uint32>(sleepDuration.count() * 1000.0f));
        }
    }
}

void Game::processEvents(bool& running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                width_ = event.window.data1;
                height_ = event.window.data2;
            }
        }

    handleMainMenuEvent(event, running);
    }
}

void Game::update(float deltaSeconds) {
    (void)deltaSeconds;
    // Placeholder for simulation updates: world ticks, input handling, and AI.
}

void Game::render() {
    // Clear with a subtle dungeon-inspired hue.
    SDL_SetRenderDrawColor(renderer_, 20, 16, 24, 255);
    SDL_RenderClear(renderer_);

    renderMainMenu();

    SDL_RenderPresent(renderer_);
}

void Game::shutdown() {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }

    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
}

void Game::handleMainMenuEvent(const SDL_Event& event, bool& running) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        const int mouseX = event.button.x;
        const int mouseY = event.button.y;
        for (const auto& button : mainMenuButtons_) {
            const bool inside = mouseX >= button.bounds.x && mouseX <= button.bounds.x + button.bounds.w &&
                                mouseY >= button.bounds.y && mouseY <= button.bounds.y + button.bounds.h;
            if (inside) {
                if (button.label == "QUIT") {
                    running = false;
                }
            }
        }
    }
}

void Game::renderMainMenu() {
    SDL_Color accent{120, 96, 160, 255};

    // Title
    const int titleScale = 4;
    const int titleWidth = static_cast<int>("DUNGEON DELVERS"s.size()) * (5 * titleScale + 1) - 1;
    const int titleX = width_ / 2 - titleWidth / 2;
    const int titleY = 80;
    drawText("DUNGEON DELVERS", titleX, titleY, titleScale, accent);

    // Buttons
    int mouseX = 0;
    int mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);

    for (const auto& button : mainMenuButtons_) {
        const bool hovered = mouseX >= button.bounds.x && mouseX <= button.bounds.x + button.bounds.w &&
                             mouseY >= button.bounds.y && mouseY <= button.bounds.y + button.bounds.h;
        drawButton(button, hovered);
    }

    // Temporary notice to indicate other game flows are disabled.
    SDL_Color noticeColor{180, 170, 200, 255};
    const std::string noticeText = "MAIN MENU ONLY BUILD";
    const int noticeScale = 2;
    const int noticeWidth = static_cast<int>(noticeText.size()) * (5 * noticeScale + 1) - 1;
    const int noticeX = width_ / 2 - noticeWidth / 2;
    const int noticeY = height_ - 80;
    drawText(noticeText, noticeX, noticeY, noticeScale, noticeColor);
}

void Game::drawButton(const Button& button, bool hovered) {
    SDL_Color fillColor = hovered ? SDL_Color{60, 48, 72, 240} : SDL_Color{40, 32, 48, 220};
    SDL_Color borderColor{160, 140, 200, 255};
    SDL_SetRenderDrawColor(renderer_, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
    SDL_RenderFillRect(renderer_, &button.bounds);

    SDL_SetRenderDrawColor(renderer_, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(renderer_, &button.bounds);

    SDL_Color textColor{230, 225, 240, 255};
    const int scale = 2;
    const int textWidth = static_cast<int>(button.label.size()) * (5 * scale + 1) - 1;
    const int textHeight = 7 * scale;
    const int textX = button.bounds.x + (button.bounds.w - textWidth) / 2;
    const int textY = button.bounds.y + (button.bounds.h - textHeight) / 2;
    drawText(button.label, textX, textY, scale, textColor);
}

void Game::drawText(const std::string& text, int x, int y, int scale, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
    int cursorX = x;
    for (char rawChar : text) {
        const auto& glyph = glyphForChar(static_cast<char>(std::toupper(rawChar)));
        for (int row = 0; row < static_cast<int>(glyph.size()); ++row) {
            for (int col = 0; col < static_cast<int>(glyph[row].size()); ++col) {
                if (glyph[row][col] == '1') {
                    SDL_Rect pixelRect{cursorX + col * scale, y + row * scale, scale, scale};
                    SDL_RenderFillRect(renderer_, &pixelRect);
                }
            }
        }
        cursorX += (5 * scale) + scale; // glyph width plus spacing
    }
}

const std::array<std::string, 7>& Game::glyphForChar(char c) const {
    auto it = FONT.find(static_cast<char>(std::toupper(c)));
    if (it != FONT.end()) {
        return it->second;
    }
    return FONT.at(' ');
}
