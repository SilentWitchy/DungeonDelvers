#include "sim/World.h"

namespace dd {

    void World::Init(int sx, int sy, int zAbove, int zBelow) {
        m_sx = sx;
        m_sy = sy;
        m_zmin = -zBelow;
        m_zmax = zAbove;

        m_tiles.assign((size_t)m_sx * (size_t)m_sy * (size_t)ZCount(), Tile{});

        // Base stratification
        for (int z = m_zmin; z <= m_zmax; ++z) {
            for (int y = 0; y < m_sy; ++y) {
                for (int x = 0; x < m_sx; ++x) {
                    Tile& t = Get(x, y, z);
                    if (z > 0) t.type = TileType::Air;
                    else if (z == 0) t.type = TileType::Soil;
                    else t.type = TileType::Stone;
                }
            }
        }

        // TEMP: Abandoned house set-piece
        const int z0 = 0;
        const int cx = m_sx / 2;
        const int cy = m_sy / 2;
        const int houseW = 20;
        const int houseH = 14;

        // Floor
        for (int y = cy - houseH / 2; y <= cy + houseH / 2; ++y) {
            for (int x = cx - houseW / 2; x <= cx + houseW / 2; ++x) {
                Get(x, y, z0).type = TileType::WoodFloor;
            }
        }

        // Walls
        for (int x = cx - houseW / 2; x <= cx + houseW / 2; ++x) {
            Get(x, cy - houseH / 2, z0).type = TileType::PlankWall;
            Get(x, cy + houseH / 2, z0).type = TileType::PlankWall;
        }
        for (int y = cy - houseH / 2; y <= cy + houseH / 2; ++y) {
            Get(cx - houseW / 2, y, z0).type = TileType::PlankWall;
            Get(cx + houseW / 2, y, z0).type = TileType::PlankWall;
        }

        // Basement patch (z=-1)
        const int zb = -1;
        for (int y = cy - 4; y <= cy + 4; ++y) {
            for (int x = cx - 6; x <= cx + 6; ++x) {
                Get(x, y, zb).type = TileType::Soil;
            }
        }
    }

    int World::Idx(int x, int y, int z) const {
        const int zi = z - m_zmin;
        return (zi * m_sy + y) * m_sx + x;
    }

    const Tile& World::Get(int x, int y, int z) const {
        assert(x >= 0 && x < m_sx && y >= 0 && y < m_sy && z >= m_zmin && z <= m_zmax);
        return m_tiles[(size_t)Idx(x, y, z)];
    }

    Tile& World::Get(int x, int y, int z) {
        assert(x >= 0 && x < m_sx && y >= 0 && y < m_sy && z >= m_zmin && z <= m_zmax);
        return m_tiles[(size_t)Idx(x, y, z)];
    }

} // namespace dd
