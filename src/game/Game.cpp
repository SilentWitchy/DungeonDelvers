#include "game/Game.hpp"

#include <SDL.h>
#include <chrono>
#include <iostream>
namespace {
constexpr int TARGET_FPS = 60;
constexpr float TARGET_FRAME_TIME = 1.0f / static_cast<float>(TARGET_FPS);
}

Game::Game()
    : window_(nullptr), renderer_(nullptr), width_(0), height_(0) {}

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
        } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            width_ = event.window.data1;
            height_ = event.window.data2;
        }
    }
}

void Game::update(float deltaSeconds) {
    (void)deltaSeconds;
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
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
