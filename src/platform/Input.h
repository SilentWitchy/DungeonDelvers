#pragma once
#include <SDL.h>

namespace dd {

    struct Input final {
        // keys
        bool key_w = false, key_a = false, key_s = false, key_d = false;
        bool key_q = false, key_e = false;
        bool key_pgup = false, key_pgdn = false;

        int wheel_y = 0;

        int mouse_x = 0;
        int mouse_y = 0;
        bool mouse_left = false;
        bool mouse_left_pressed = false;

        void BeginFrame() {
            wheel_y = 0;
            key_pgup = false;
            key_pgdn = false;
            mouse_left_pressed = false;
        }

        void ProcessEvent(const SDL_Event& e) {
            if (e.type == SDL_MOUSEWHEEL) {
                wheel_y += e.wheel.y;
            }

            if (e.type == SDL_MOUSEMOTION) {
                mouse_x = e.motion.x;
                mouse_y = e.motion.y;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
                const bool down = (e.type == SDL_MOUSEBUTTONDOWN);
                if (e.button.button == SDL_BUTTON_LEFT) {
                    mouse_left = down;
                    if (down) mouse_left_pressed = true;
                }
            }

            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                const bool down = (e.type == SDL_KEYDOWN);
                const SDL_Keycode k = e.key.keysym.sym;

                if (k == SDLK_w) key_w = down;
                if (k == SDLK_a) key_a = down;
                if (k == SDLK_s) key_s = down;
                if (k == SDLK_d) key_d = down;

                if (k == SDLK_q) key_q = down;
                if (k == SDLK_e) key_e = down;

                if (down && k == SDLK_PAGEUP) key_pgup = true;
                if (down && k == SDLK_PAGEDOWN) key_pgdn = true;
            }
        }
    };

} // namespace dd
