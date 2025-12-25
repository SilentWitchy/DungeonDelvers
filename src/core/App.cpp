#include "core/App.h"

#include <algorithm>
#include <string>

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

        // Temporarily center on the middle of the map while combat systems are disabled.
        m_activeZ = 0;

        m_cam.Pan(
            float(m_world.SizeX() * m_cfg.tile_px) * 0.5f,
            float(m_world.SizeY() * m_cfg.tile_px) * 0.5f
        );

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
        // Simulation systems (core, spawners, invaders) are disabled while focusing on
        // terrain work. This stub remains to keep the fixed-timestep loop structure.
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

        // Combat entities are disabled while focusing on world generation.

        m_batch.EndAndDraw();

        SDL_GL_SwapWindow(m_sdl.Window());
    }

    void App::HandleResize(int w, int h) {
        m_cam.SetViewport((float)w, (float)h);
        gl33::Viewport(0, 0, w, h);
    }

} // namespace dd
