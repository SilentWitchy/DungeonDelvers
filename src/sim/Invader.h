#pragma once
#include <cstdint>

namespace dd {

	enum class InvaderKind : uint8_t { Delver = 0, Vermin = 1 };

	struct Invader final {
		int x = 0, y = 0, z = 0;
		InvaderKind kind = InvaderKind::Delver;
		int hp = 20;

		int move_cooldown = 0;
		int move_reset = 6;
	};

} // namespace dd
