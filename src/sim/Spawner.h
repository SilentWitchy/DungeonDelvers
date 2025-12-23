#pragma once
#include <string>

namespace dd {

	struct Spawner final {
		int x = 0, y = 0, z = 0;
		std::string kind; // "rats", "wasps", "ghouls"

		int tier = 1;
		int cap = 5;
		float spawn_rate = 1.0f;
		float stat_mult = 1.0f;

		int cooldown_ticks = 0;
		int cooldown_reset = 30;
	};

} // namespace dd
