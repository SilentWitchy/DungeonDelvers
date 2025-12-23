#pragma once
#include <cstdint>

namespace dd {

	enum class MobKind : uint8_t { Rat = 0, Wasp = 1, Ghoul = 2 };

	struct Mob final {
		int x = 0, y = 0, z = 0;
		MobKind kind = MobKind::Rat;
		int hp = 10;
	};

} // namespace dd
