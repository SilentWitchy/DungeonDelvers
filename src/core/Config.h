#pragma once
#include <cstdint>

namespace dd {
	struct Config final {
		int window_w = 1280;
		int window_h = 720;
		const char* title = "Dungeon Delvers";

		// Simulation
		int sim_ticks_per_sec = 30; // “DF-like” discrete ticks; change freely later

		// World
		int world_x = 128;
		int world_y = 128;
		int z_above = 20;
		int z_below = 40; // underground
		int tile_px = 16;
	};
} // namespace dd
