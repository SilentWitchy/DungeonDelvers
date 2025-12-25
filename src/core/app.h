#pragma once

#include <vector>
#include <string>

#include "platform/SDLContext.h"
#include "platform/Input.h"

#include "render/Camera.h"
#include "render/SpriteBatch.h"
#include "render/Shader.h"
#include "render/Texture.h"

#include "sim/World.h"

namespace dd {

        enum class AppMode {
                Menu,
                Playing
        };

	struct AppConfig final {
		const char* title = "DungeonDelvers";

		int window_w = 1280;
		int window_h = 720;

		int tile_px = 16;

		int world_x = 96;
		int world_y = 64;

		int z_above = 20;
		int z_below = 40;
	};

	class App final {
	public:
		explicit App(AppConfig cfg);
		int Run();

        private:
                void StepSim();
                void RenderFrame();
                void HandleResize(int w, int h);

                void RenderMenu();
                void RenderWorld();
                void RenderText(float x, float y, float scale, const char* text, float r, float g, float b, float a);
                Mat4 ScreenOrtho() const;
                bool MenuButtonPressed(float x, float y, float w, float h) const;

        private:
                AppConfig m_cfg{};

                SDLContext m_sdl{};
                Input m_in{};

                Camera m_cam{};
                Shader m_shader{};
                Texture m_white{};
                SpriteBatch m_batch{};

                World m_world{};

                AppMode m_mode = AppMode::Menu;

                int m_activeZ = 0;
                bool m_running = true;

                float m_viewW = 0.0f;
                float m_viewH = 0.0f;
        };

} // namespace dd
