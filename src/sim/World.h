#pragma once
#include <vector>
#include <cassert>

namespace dd {

	enum class TileType : unsigned char {
		Air = 0,
		Soil,
		Stone,
		WoodFloor,
		PlankWall
	};

	struct Tile final {
		TileType type = TileType::Air;
	};

	class World final {
	public:
		void Init(int sx, int sy, int zAbove, int zBelow);

		int SizeX() const { return m_sx; }
		int SizeY() const { return m_sy; }
		int ZMin() const { return m_zmin; }
		int ZMax() const { return m_zmax; }

		const Tile& Get(int x, int y, int z) const;
		Tile& Get(int x, int y, int z);

	private:
		int ZCount() const { return m_zmax - m_zmin + 1; }
		int Idx(int x, int y, int z) const;

	private:
		int m_sx = 0;
		int m_sy = 0;
		int m_zmin = 0;
		int m_zmax = 0;

		std::vector<Tile> m_tiles{};
	};

} // namespace dd
