#include "render/Camera.h"

namespace dd {

    Mat4 Mat4::Identity() {
        Mat4 o{};
        o.m = { 1,0,0,0,
               0,1,0,0,
               0,0,1,0,
               0,0,0,1 };
        return o;
    }

    Mat4 Mat4::Ortho(float l, float r, float b, float t, float n, float f) {
        Mat4 o{};
        const float rl = (r - l);
        const float tb = (t - b);
        const float fn = (f - n);

        o.m = {
          2.0f / rl, 0,        0,        0,
          0,       2.0f / tb,  0,        0,
          0,       0,       -2.0f / fn,  0,
          -(r + l) / rl, -(t + b) / tb, -(f + n) / fn, 1
        };
        return o;
    }

    Mat4 operator*(const Mat4& a, const Mat4& b) {
        Mat4 o{};
        for (int c = 0; c < 4; ++c) {
            for (int r = 0; r < 4; ++r) {
                o.m[c * 4 + r] =
                    a.m[0 * 4 + r] * b.m[c * 4 + 0] +
                    a.m[1 * 4 + r] * b.m[c * 4 + 1] +
                    a.m[2 * 4 + r] * b.m[c * 4 + 2] +
                    a.m[3 * 4 + r] * b.m[c * 4 + 3];
            }
        }
        return o;
    }

    Mat4 Camera::ViewProj() const {
        const float halfW = (m_vw * 0.5f) * m_zoom;
        const float halfH = (m_vh * 0.5f) * m_zoom;

        const float l = m_cx - halfW;
        const float r = m_cx + halfW;
        const float t = m_cy - halfH;
        const float b = m_cy + halfH;

        return Mat4::Ortho(l, r, b, t, -1.0f, 1.0f);
    }

} // namespace dd
