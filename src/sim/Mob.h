#pragma once
#include <cstdint>

namespace dd {

	enum class MobKind : std::uint8_t {
		Rat = 0,
		Wasp = 1,
		Ghoul = 2
	};

	struct Mob final {
		int x = 0;
		int y = 0;
		int z = 0;

		MobKind kind = MobKind::Rat;
		int hp = 10;

		int spawner_id = -1; // which Spawner index created this mob
	};

} // namespace dd
