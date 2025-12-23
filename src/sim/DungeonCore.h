#pragma once

namespace dd {

	struct DungeonCore final {
		int x = 0, y = 0, z = 0;

		int hp = 100;
		int hp_max = 100;

		int mana = 0;
		int lifetime_mana = 0;
	};

} // namespace dd
