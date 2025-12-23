#pragma once
#include <array>

namespace dd {

	struct Mat4 final {
		std::array<float, 16> m{};

		static Mat4 Identity();
		static Mat4 Ortho(float l, float r, float b, float t, float n, float f);

		friend Mat4 operator*(const Mat4& a, const Mat4& b);
	};

	class Camera final {
	public:
		void SetViewport(float w, float h) { m_vw = w; m_vh = h; }
		void Pan(float world_px_x, float world_px_y) { m_cx = world_px_x; m_cy = world_px_y; }
		void Move(float dx, float dy) { m_cx += dx; m_cy += dy; }

		void SetZoom(float z) { m_zoom = (z <= 0.001f) ? 0.001f : z; }
		void ZoomBy(float factor) { SetZoom(m_zoom * factor); }

		Mat4 ViewProj() const;

	private:
		float m_vw = 1280.0f;
		float m_vh = 720.0f;

		float m_cx = 0.0f;
		float m_cy = 0.0f;

		float m_zoom = 1.0f;
	};

} // namespace dd
