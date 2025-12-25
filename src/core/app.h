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

	private:
		AppConfig m_cfg{};

		SDLContext m_sdl{};
		Input m_in{};

		Camera m_cam{};
		Shader m_shader{};
		Texture m_white{};
		SpriteBatch m_batch{};

                World m_world{};

		int m_activeZ = 0;
		bool m_running = true;
	};

} // namespace dd
