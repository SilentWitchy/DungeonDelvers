#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "core/App.h"

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	dd::AppConfig cfg{};
	dd::App app(cfg);
	return app.Run();
}
