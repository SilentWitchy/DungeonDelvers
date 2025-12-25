#include "core/App.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <string>

#include <SDL.h>

#include "render/GL33.h"
#include "core/Log.h"

namespace dd {

    // ---------------------- helpers ----------------------

    struct Glyph {
        uint8_t rows[7];
    };

    static constexpr Glyph MakeGlyph(std::initializer_list<uint8_t> rows) {
        Glyph g{};
        size_t i = 0;
        for (uint8_t r : rows) {
            g.rows[i++] = r;
        }
        return g;
    }

    static const Glyph* LookupGlyph(char c) {
        static constexpr Glyph SPACE = MakeGlyph({ 0,0,0,0,0,0,0 });
        static constexpr Glyph A = MakeGlyph({ 0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11 });
        static constexpr Glyph D = MakeGlyph({ 0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E });
        static constexpr Glyph E = MakeGlyph({ 0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F });
        static constexpr Glyph G = MakeGlyph({ 0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0E });
        static constexpr Glyph I = MakeGlyph({ 0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E });
        static constexpr Glyph L = MakeGlyph({ 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F });
        static constexpr Glyph N = MakeGlyph({ 0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11 });
        static constexpr Glyph O = MakeGlyph({ 0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E });
        static constexpr Glyph Q = MakeGlyph({ 0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D });
        static constexpr Glyph R = MakeGlyph({ 0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11 });
        static constexpr Glyph S = MakeGlyph({ 0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E });
        static constexpr Glyph T = MakeGlyph({ 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04 });
        static constexpr Glyph U = MakeGlyph({ 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E });
        static constexpr Glyph V = MakeGlyph({ 0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04 });

        switch (std::toupper(static_cast<unsigned char>(c))) {
        case 'A': return &A;
        case 'D': return &D;
        case 'E': return &E;
        case 'G': return &G;
        case 'I': return &I;
        case 'L': return &L;
        case 'N': return &N;
        case 'O': return &O;
        case 'Q': return &Q;
        case 'R': return &R;
        case 'S': return &S;
        case 'T': return &T;
        case 'U': return &U;
        case 'V': return &V;
        case ' ': return &SPACE;
        default:  return nullptr;
        }
    }

    struct ButtonRect {
        float x, y, w, h;
    };

    static ButtonRect StartButton(float vw, float vh) {
        const float w = 240.0f;
        const float h = 60.0f;
        return { (vw - w) * 0.5f, vh * 0.55f, w, h };
    }

    static ButtonRect QuitButton(float vw, float vh) {
        const float w = 240.0f;
        const float h = 60.0f;
        const float startY = vh * 0.55f;
        const float spacing = 20.0f;
        return { (vw - w) * 0.5f, startY + h + spacing, w, h };
    }

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
        m_viewW = (float)m_cfg.window_w;
        m_viewH = (float)m_cfg.window_h;

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

            if (m_mode == AppMode::Menu) {
                const ButtonRect start = StartButton(m_viewW, m_viewH);
                const ButtonRect quit = QuitButton(m_viewW, m_viewH);

                if (MenuButtonPressed(start.x, start.y, start.w, start.h)) {
                    m_mode = AppMode::Playing;
                }

                if (MenuButtonPressed(quit.x, quit.y, quit.w, quit.h)) {
                    m_running = false;
                }

                SDL_SetWindowTitle(m_sdl.Window(), "Dungeon Delvers - Menu");
            } else {
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

        m_shader.Bind();
        m_shader.SetInt("u_tex", 0);

        m_white.Bind(0);

        m_batch.Begin();

        if (m_mode == AppMode::Menu) {
            RenderMenu();
        } else {
            RenderWorld();
        }

        m_batch.EndAndDraw();

        SDL_GL_SwapWindow(m_sdl.Window());
    }

    void App::RenderMenu() {
        m_shader.SetMat4("u_viewproj", ScreenOrtho());

        // Dim background
        m_batch.PushQuad(0, 0, m_viewW, m_viewH, 0.05f, 0.05f, 0.07f, 1.0f);

        // Title
        const float titleScale = 6.0f;
        const float titleWidth = 14.0f * 6.0f * titleScale; // approximate length
        const float titleX = (m_viewW - titleWidth) * 0.5f;
        const float titleY = m_viewH * 0.25f;
        RenderText(titleX, titleY, titleScale, "Dungeon Delvers", 0.85f, 0.85f, 0.92f, 1.0f);

        // Buttons
        const ButtonRect start = StartButton(m_viewW, m_viewH);
        const ButtonRect quit = QuitButton(m_viewW, m_viewH);

        const auto drawButton = [this](const ButtonRect& r, const char* label) {
            m_batch.PushQuad(r.x, r.y, r.w, r.h, 0.18f, 0.20f, 0.24f, 1.0f);
            m_batch.PushQuad(r.x + 2.0f, r.y + 2.0f, r.w - 4.0f, r.h - 4.0f, 0.12f, 0.13f, 0.16f, 1.0f);
            const float textScale = 4.0f;
            const float textWidth = std::strlen(label) * 6.0f * textScale;
            const float tx = r.x + (r.w - textWidth) * 0.5f;
            const float ty = r.y + (r.h - 7.0f * textScale) * 0.5f;
            RenderText(tx, ty, textScale, label, 0.85f, 0.85f, 0.92f, 1.0f);
        };

        drawButton(start, "Start");
        drawButton(quit, "Quit");
    }

    void App::RenderWorld() {
        const float tile = float(m_cfg.tile_px);
        m_shader.SetMat4("u_viewproj", m_cam.ViewProj());

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
    }

    void App::RenderText(float x, float y, float scale, const char* text, float r, float g, float b, float a) {
        float penX = x;
        const float penY = y;

        for (size_t i = 0; text[i] != '\0'; ++i) {
            const Glyph* glyph = LookupGlyph(text[i]);
            if (!glyph) {
                penX += 6.0f * scale;
                continue;
            }

            if (text[i] == ' ') {
                penX += 6.0f * scale;
                continue;
            }

            for (int row = 0; row < 7; ++row) {
                const uint8_t bits = glyph->rows[row];
                for (int col = 0; col < 5; ++col) {
                    if (bits & (1 << (4 - col))) {
                        m_batch.PushQuad(penX + col * scale, penY + row * scale, scale, scale, r, g, b, a);
                    }
                }
            }

            penX += 6.0f * scale;
        }
    }

    Mat4 App::ScreenOrtho() const {
        return Mat4::Ortho(0.0f, m_viewW, m_viewH, 0.0f, -1.0f, 1.0f);
    }

    bool App::MenuButtonPressed(float x, float y, float w, float h) const {
        if (!m_in.mouse_left_pressed) return false;
        const float mx = (float)m_in.mouse_x;
        const float my = (float)m_in.mouse_y;
        return mx >= x && mx <= (x + w) && my >= y && my <= (y + h);
    }

    void App::HandleResize(int w, int h) {
        m_cam.SetViewport((float)w, (float)h);
        m_viewW = (float)w;
        m_viewH = (float)h;
        gl33::Viewport(0, 0, w, h);
    }

} // namespace dd
