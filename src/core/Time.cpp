#include "core/Time.h"
#include <SDL.h>

namespace dd {
	uint64_t TicksNow() { return SDL_GetPerformanceCounter(); }
	double SecondsNow() { return double(SDL_GetPerformanceCounter()) / double(SDL_GetPerformanceFrequency()); }
}
