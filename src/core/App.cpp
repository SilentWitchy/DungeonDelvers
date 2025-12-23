#include "core/App.h"

#include <algorithm>
#include <string>
#include <vector>

#include <SDL.h>

#include "render/GL33.h"
#include "core/Log.h"

namespace dd {

    // ---------------------- helpers ----------------------

    static void SetGLDefaults(int w, int h) {
        gl33::Viewport(0, 0, w, h);
        gl33::Disable(GL_DEPTH_TEST);
        gl33::Enable(GL_BLEND);
        gl33::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    static bool IsWalkable(TileType t) {
        return t != TileType::PlankWall;
    }

    static int Idx2D(int x, int y, int sx) {
        return y * sx + x;
    }

    // ---------------------- App ----------------------

    App::App(AppConfig cfg) : m_cfg(cfg) {}

    int App::Run() {
        if (!m_sdl.Init(m_cfg.title, m_cfg.window_w, m_cfg.window_h)) {
            LogError("SDL init failed");
            return 1;
        }

        if (!gl33::Load()) {
            LogError("OpenGL function load failed");
            return 2;
        }

        SetGLDefaults(m_cfg.window_w, m_cfg.window_h);

        // Camera/render init
        m_cam.SetViewport((float)m_cfg.window_w, (float)m_cfg.window_h);
        m_cam.SetZoom(1.0f);

        m_white.CreateWhite();

        if (!m_shader.LoadFromFiles("assets/shaders/sprite.vert", "assets/shaders/sprite.frag")) {
            LogError("Shader load failed. Check that assets/shaders were copied to the build folder.");
            return 3;
        }

        m_batch.Init();

        // World init
        m_world.Init(m_cfg.world_x, m_cfg.world_y, m_cfg.z_above, m_cfg.z_below);

        m_core = DungeonCore{
          m_cfg.world_x / 2,
          m_cfg.world_y / 2,
          0,
          100, 100,
          0, 0
        };

        m_activeZ = m_core.z;

        // Center camera on core (world pixels)
        m_cam.Pan(
            float(m_core.x * m_cfg.tile_px),
            float(m_core.y * m_cfg.tile_px)
        );

        // Spawners
        m_spawners.clear();
        m_spawners.push_back(Spawner{ m_core.x + 2, m_core.y, 0, "rats" });
        m_spawners.push_back(Spawner{ m_core.x - 2, m_core.y, 0, "wasps" });

        for (auto& s : m_spawners) {
            s.cooldown_reset = 30;
            s.cooldown_ticks = 10;
            s.cap = 8;
        }

        m_mobs.clear();

        // Invaders/pathing
        m_invaders.clear();
        m_dist.assign((size_t)m_world.SizeX() * (size_t)m_world.SizeY(), -1);

        m_invader_spawn_reset = 90;
        m_invader_spawn_ticks = 30;

        // Main loop timing
        const int SIM_TPS = 30;
        const double SIM_DT = 1.0 / double(SIM_TPS);

        Uint64 last = SDL_GetPerformanceCounter();
        double acc = 0.0;

        while (m_running) {
            Uint64 now = SDL_GetPerformanceCounter();
            double dt = double(now - last) / double(SDL_GetPerformanceFrequency());
            last = now;
            acc += dt;

            // Input
            m_in.BeginFrame();

            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) m_running = false;

                if (e.type == SDL_WINDOWEVENT &&
                    e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    HandleResize(e.window.data1, e.window.data2);
                }

                m_in.ProcessEvent(e);
            }

            // Camera movement
            const float camSpeed = 400.0f * float(dt);
            if (m_in.key_w) m_cam.Move(0, -camSpeed);
            if (m_in.key_s) m_cam.Move(0, camSpeed);
            if (m_in.key_a) m_cam.Move(-camSpeed, 0);
            if (m_in.key_d) m_cam.Move(camSpeed, 0);

            if (m_in.key_q) m_cam.ZoomBy(1.02f);
            if (m_in.key_e) m_cam.ZoomBy(0.98f);

            // Z controls
            if (m_in.wheel_y != 0) m_activeZ += m_in.wheel_y;
            if (m_in.key_pgup) m_activeZ += 1;
            if (m_in.key_pgdn) m_activeZ -= 1;

            m_activeZ = std::clamp(m_activeZ, m_world.ZMin(), m_world.ZMax());

            {
                std::string title = std::string(m_cfg.title) + "  |  Z=" + std::to_string(m_activeZ);
                SDL_SetWindowTitle(m_sdl.Window(), title.c_str());
            }

            // Fixed-step sim
            while (acc >= SIM_DT) {
                StepSim();
                acc -= SIM_DT;
            }

            RenderFrame();
        }

        return 0;
    }

    // ---------------------- Simulation ----------------------

    void App::StepSim() {
        const int z = m_core.z;
        const int sx = m_world.SizeX();
        const int sy = m_world.SizeY();

        if ((int)m_dist.size() != sx * sy) {
            m_dist.assign((size_t)sx * (size_t)sy, -1);
        }

        // ---- 1) Spawn mobs from spawners (minimal) ----
        auto count_kind_on_z = [&](const Spawner& sp) -> int {
            int c = 0;
            for (const auto& m : m_mobs) {
                if (m.z != sp.z) continue;
                if (sp.kind == "rats" && m.kind == MobKind::Rat) c++;
                if (sp.kind == "wasps" && m.kind == MobKind::Wasp) c++;
                if (sp.kind == "ghouls" && m.kind == MobKind::Ghoul) c++;
            }
            return c;
            };

        auto spawn_mob_near = [&](const Spawner& sp) {
            Mob mob{};
            mob.z = sp.z;

            if (sp.kind == "rats") { mob.kind = MobKind::Rat; mob.hp = 10; }
            else if (sp.kind == "wasps") { mob.kind = MobKind::Wasp; mob.hp = 6; }
            else { mob.kind = MobKind::Ghoul; mob.hp = 20; }

            bool placed = false;
            for (int i = 0; i < 12; ++i) {
                const int seed = int(m_mobs.size()) * 17 + i * 31 + sp.x * 7 + sp.y * 13;
                const int ox = (seed % 7) - 3;
                const int oy = ((seed / 7) % 7) - 3;

                const int x = sp.x + ox;
                const int y = sp.y + oy;
                if (x < 0 || x >= sx || y < 0 || y >= sy) continue;

                mob.x = x;
                mob.y = y;
                placed = true;
                break;
            }

            if (!placed) { mob.x = sp.x; mob.y = sp.y; }
            m_mobs.push_back(mob);
            };

        for (auto& sp : m_spawners) {
            if (sp.cooldown_ticks > 0) sp.cooldown_ticks--;
            if (sp.cooldown_ticks <= 0) {
                if (count_kind_on_z(sp) < sp.cap) spawn_mob_near(sp);
                sp.cooldown_ticks = sp.cooldown_reset;
            }
        }

        // ---- 2) Spawn invaders from edges ----
        if (m_invader_spawn_ticks > 0) m_invader_spawn_ticks--;
        if (m_invader_spawn_ticks <= 0) {
            Invader inv{};
            inv.z = z;
            inv.kind = InvaderKind::Delver;
            inv.hp = 20;
            inv.move_reset = 6;
            inv.move_cooldown = 0;

            const int seed = (int)m_invaders.size() * 101 + m_core.x * 17 + m_core.y * 29;
            const int edge = seed % 4;

            if (edge == 0) { inv.x = (seed * 13) % sx; inv.y = 0; }
            else if (edge == 1) { inv.x = (seed * 13) % sx; inv.y = sy - 1; }
            else if (edge == 2) { inv.x = 0; inv.y = (seed * 19) % sy; }
            else { inv.x = sx - 1; inv.y = (seed * 19) % sy; }

            // Nudge off walls if needed
            for (int i = 0; i < 8; ++i) {
                if (IsWalkable(m_world.Get(inv.x, inv.y, z).type)) break;
                if (inv.x == 0) inv.x++;
                else if (inv.x == sx - 1) inv.x--;
                if (inv.y == 0) inv.y++;
                else if (inv.y == sy - 1) inv.y--;
            }

            m_invaders.push_back(inv);
            m_invader_spawn_ticks = m_invader_spawn_reset;
        }

        // ---- 3) BFS distance field from core ----
        std::fill(m_dist.begin(), m_dist.end(), -1);

        struct Node { int x, y; };
        std::vector<Node> q;
        q.reserve((size_t)sx * (size_t)sy);

        m_dist[Idx2D(m_core.x, m_core.y, sx)] = 0;
        q.push_back({ m_core.x, m_core.y });

        for (size_t i = 0; i < q.size(); ++i) {
            Node n = q[i];
            const int baseD = m_dist[Idx2D(n.x, n.y, sx)];

            const int nx[4] = { n.x + 1, n.x - 1, n.x,     n.x };
            const int ny[4] = { n.y,     n.y,     n.y + 1, n.y - 1 };

            for (int k = 0; k < 4; ++k) {
                const int x = nx[k];
                const int y = ny[k];
                if (x < 0 || x >= sx || y < 0 || y >= sy) continue;

                const int idx = Idx2D(x, y, sx);
                if (m_dist[idx] != -1) continue;
                if (!IsWalkable(m_world.Get(x, y, z).type)) continue;

                m_dist[idx] = baseD + 1;
                q.push_back({ x, y });
            }
        }

        // ---- 4) Move invaders downhill ----
        for (auto& inv : m_invaders) {
            if (inv.z != z) continue;

            if (inv.move_cooldown > 0) {
                inv.move_cooldown--;
                continue;
            }

            const int hereIdx = Idx2D(inv.x, inv.y, sx);
            const int hereD = m_dist[hereIdx];

            if (hereD <= 0) {
                inv.move_cooldown = inv.move_reset;
                continue;
            }

            int bestX = inv.x;
            int bestY = inv.y;
            int bestD = hereD;

            const int nx[4] = { inv.x + 1, inv.x - 1, inv.x,     inv.x };
            const int ny[4] = { inv.y,     inv.y,     inv.y + 1, inv.y - 1 };

            for (int k = 0; k < 4; ++k) {
                const int x = nx[k];
                const int y = ny[k];
                if (x < 0 || x >= sx || y < 0 || y >= sy) continue;

                const int idx = Idx2D(x, y, sx);
                const int d = m_dist[idx];
                if (d != -1 && d < bestD) {
                    bestD = d;
                    bestX = x;
                    bestY = y;
                }
            }

            inv.x = bestX;
            inv.y = bestY;
            inv.move_cooldown = inv.move_reset;
        }
    }

    // ---------------------- Rendering ----------------------

    void App::RenderFrame() {
        // Make clear color visible (not pure black)
        gl33::ClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        gl33::Clear(GL_COLOR_BUFFER_BIT);

        const float tile = float(m_cfg.tile_px);

        m_shader.Bind();
        m_shader.SetMat4("u_viewproj", m_cam.ViewProj());
        m_shader.SetInt("u_tex", 0);

        m_white.Bind(0);

        m_batch.Begin();

        // Tiles
        for (int y = 0; y < m_world.SizeY(); ++y) {
            for (int x = 0; x < m_world.SizeX(); ++x) {
                const Tile& t = m_world.Get(x, y, m_activeZ);

                float r = 0, g = 0, b = 0, a = 1;

                switch (t.type) {
                case TileType::Air:        r = 0.10f; g = 0.12f; b = 0.16f; break;
                case TileType::Soil:       r = 0.45f; g = 0.30f; b = 0.15f; break;
                case TileType::Stone:      r = 0.50f; g = 0.50f; b = 0.50f; break;
                case TileType::WoodFloor:  r = 0.55f; g = 0.40f; b = 0.25f; break;
                case TileType::PlankWall:  r = 0.30f; g = 0.20f; b = 0.10f; break;
                }

                m_batch.PushQuad(x * tile, y * tile, tile, tile, r, g, b, a);
            }
        }

        // Core
        if (m_core.z == m_activeZ) {
            m_batch.PushQuad(m_core.x * tile, m_core.y * tile, tile, tile, 0.80f, 0.20f, 0.90f, 1.0f);
        }

        // Spawners (blue)
        for (const auto& s : m_spawners) {
            if (s.z != m_activeZ) continue;
            m_batch.PushQuad(s.x * tile, s.y * tile, tile, tile, 0.20f, 0.60f, 0.90f, 1.0f);
        }

        // Mobs
        for (const auto& m : m_mobs) {
            if (m.z != m_activeZ) continue;

            float r = 1, g = 1, b = 1;
            if (m.kind == MobKind::Rat) { r = 0.9f; g = 0.9f; b = 0.9f; }
            if (m.kind == MobKind::Wasp) { r = 0.95f; g = 0.85f; b = 0.2f; }
            if (m.kind == MobKind::Ghoul) { r = 0.2f; g = 0.9f; b = 0.2f; }

            m_batch.PushQuad(m.x * tile, m.y * tile, tile, tile, r, g, b, 1.0f);
        }

        // Invaders (red)
        for (const auto& inv : m_invaders) {
            if (inv.z != m_activeZ) continue;
            m_batch.PushQuad(inv.x * tile, inv.y * tile, tile, tile, 0.95f, 0.15f, 0.15f, 1.0f);
        }

        m_batch.EndAndDraw();

        SDL_GL_SwapWindow(m_sdl.Window());
    }

    void App::HandleResize(int w, int h) {
        m_cam.SetViewport((float)w, (float)h);
        gl33::Viewport(0, 0, w, h);
    }

} // namespace dd
