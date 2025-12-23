#pragma once
#include <SDL.h>

namespace dd {

	class SDLContext final {
	public:
		bool Init(const char* title, int w, int h);

		SDL_Window* Window() const { return m_win; }
		SDL_GLContext GL() const { return m_gl; }

	private:
		SDL_Window* m_win = nullptr;
		SDL_GLContext m_gl = nullptr;
	};

} // namespace dd
