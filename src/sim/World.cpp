#include "sim/World.h"

#include <algorithm>
#include <cmath>

namespace dd {

    void World::Init(int sx, int sy, int zAbove, int zBelow) {
        m_sx = sx;
        m_sy = sy;
        m_zmin = -zBelow;
        m_zmax = zAbove;

        m_tiles.assign((size_t)m_sx * (size_t)m_sy * (size_t)ZCount(), Tile{});

        std::vector<int> surfaceHeights((size_t)m_sx * (size_t)m_sy, 0);

        const float maxHeight = float(m_zmax) * 0.6f; // leave some air above the tallest peaks

        // Rolling hills + a few higher ridges that can pierce the air layers
        for (int y = 0; y < m_sy; ++y) {
            for (int x = 0; x < m_sx; ++x) {
                const float nx = float(x) / float(std::max(1, m_sx - 1));
                const float ny = float(y) / float(std::max(1, m_sy - 1));

                // Layer a few simple trigonometric waves to approximate low-frequency noise
                const float ridge = std::sin(nx * 6.2831853f) + std::cos(ny * 6.2831853f);
                const float diagonal = std::sin((nx + ny) * 10.0f);
                const float offsetRidge = std::cos((nx - ny) * 7.0f);

                // Normalize to ~[0,1]
                const float raw = ridge * 0.5f + diagonal * 0.35f + offsetRidge * 0.25f;
                const float normalized = std::clamp((raw + 1.1f) / 2.2f, 0.0f, 1.0f);

                const int surface = std::clamp(int(std::round(normalized * maxHeight)), 0, m_zmax);
                surfaceHeights[(size_t)y * (size_t)m_sx + (size_t)x] = surface;
            }
        }

        // Base stratification using the generated height map
        for (int y = 0; y < m_sy; ++y) {
            for (int x = 0; x < m_sx; ++x) {
                const int surface = surfaceHeights[(size_t)y * (size_t)m_sx + (size_t)x];
                for (int z = m_zmin; z <= m_zmax; ++z) {
                    Tile& t = Get(x, y, z);

                    if (z > surface) {
                        t.type = TileType::Air;
                    }
                    else if (z >= std::max(surface - 2, 0)) {
                        t.type = TileType::Soil; // a few layers of dirt near the surface
                    }
                    else if (z >= 0) {
                        t.type = TileType::Stone; // solid stone above the zero plane when buried by tall hills
                    }
                    else {
                        t.type = TileType::Stone; // deep underground
                    }
                }
            }
        }

        // TEMP: Abandoned house set-piece
        const int z0 = 0;
        const int cx = m_sx / 2;
        const int cy = m_sy / 2;
        const int houseW = 20;
        const int houseH = 14;

        // Level out terrain under the house
        for (int y = cy - houseH / 2; y <= cy + houseH / 2; ++y) {
            for (int x = cx - houseW / 2; x <= cx + houseW / 2; ++x) {
                for (int z = 0; z <= m_zmax; ++z) {
                    Get(x, y, z).type = (z == 0) ? TileType::Soil : TileType::Air;
                }
            }
        }

        // Floor
        for (int y = cy - houseH / 2; y <= cy + houseH / 2; ++y) {
            for (int x = cx - houseW / 2; x <= cx + houseW / 2; ++x) {
                Get(x, y, z0).type = TileType::WoodFloor;
            }
        }

        // Base stratification using the generated height map
        for (int y = 0; y < m_sy; ++y) {
            for (int x = 0; x < m_sx; ++x) {
                const int surface = surfaceHeights[(size_t)y * (size_t)m_sx + (size_t)x];
                for (int z = m_zmin; z <= m_zmax; ++z) {
                    Tile& t = Get(x, y, z);

                    if (z > surface) {
                        t.type = TileType::Air;
                    }
                    else if (z >= std::max(surface - 2, 0)) {
                        t.type = TileType::Soil; // a few layers of dirt near the surface
                    }
                    else if (z >= 0) {
                        t.type = TileType::Stone; // solid stone above the zero plane when buried by tall hills
                    }
                    else {
                        t.type = TileType::Stone; // deep underground
                    }
                }
            }
        }

        // The house set-piece is disabled while focusing on terrain generation.
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
