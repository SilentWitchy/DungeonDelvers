#pragma once
#include <cstdint>

namespace dd {

	enum class TileType : uint8_t {
		Air = 0,
		Soil,
		Stone,
		WoodFloor,
		PlankWall
	};

	struct Tile final {
		TileType type = TileType::Air;
	};

} // namespace dd
