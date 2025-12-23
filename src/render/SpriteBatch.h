#pragma once
#include <vector>
#include <cstdint>

namespace dd {

    class SpriteBatch final {
    public:
        void Init();

        void Begin();
        void PushQuad(float x, float y, float w, float h, float r, float g, float b, float a);
        void EndAndDraw();

    private:
        struct Vtx {
            float x, y;
            float u, v;
            float r, g, b, a;
        };

        unsigned int m_vao = 0;
        unsigned int m_vbo = 0;

        std::vector<Vtx> m_verts{};
    };

} // namespace dd
