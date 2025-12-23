#include "platform/SDLContext.h"
#include "core/Log.h"

namespace dd {

    bool SDLContext::Init(const char* title, int w, int h) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
            LogError(std::string("SDL_Init failed: ") + SDL_GetError());
            return false;
        }

        // GL 3.3 Core
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

        m_win = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            w, h,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
        );

        if (!m_win) {
            LogError(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
            return false;
        }

        m_gl = SDL_GL_CreateContext(m_win);
        if (!m_gl) {
            LogError(std::string("SDL_GL_CreateContext failed: ") + SDL_GetError());
            return false;
        }

        SDL_GL_MakeCurrent(m_win, m_gl);
        SDL_GL_SetSwapInterval(1);

        return true;
    }

} // namespace dd
