#include "render/SpriteBatch.h"
#include "render/GL33.h"

namespace dd {

	void SpriteBatch::Init() {
		gl33::GenVertexArrays(1, &m_vao);
		gl33::BindVertexArray(m_vao);

		gl33::GenBuffers(1, &m_vbo);
		gl33::BindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// Position (loc 0): vec2
		gl33::EnableVertexAttribArray(0);
		gl33::VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vtx), (void*)offsetof(Vtx, x));

		// UV (loc 1): vec2
		gl33::EnableVertexAttribArray(1);
		gl33::VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vtx), (void*)offsetof(Vtx, u));

		// Color (loc 2): vec4
		gl33::EnableVertexAttribArray(2);
		gl33::VertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vtx), (void*)offsetof(Vtx, r));

		gl33::BindVertexArray(0);
	}

	void SpriteBatch::Begin() {
		m_verts.clear();
	}

	void SpriteBatch::PushQuad(float x, float y, float w, float h, float r, float g, float b, float a) {
		// Two triangles
		const Vtx v0{ x,     y,     0,0, r,g,b,a };
		const Vtx v1{ x + w, y,     1,0, r,g,b,a };
		const Vtx v2{ x + w, y + h, 1,1, r,g,b,a };
		const Vtx v3{ x,     y + h, 0,1, r,g,b,a };

		m_verts.push_back(v0);
		m_verts.push_back(v1);
		m_verts.push_back(v2);

		m_verts.push_back(v0);
		m_verts.push_back(v2);
		m_verts.push_back(v3);
	}

	void SpriteBatch::EndAndDraw() {
		gl33::BindVertexArray(m_vao);
		gl33::BindBuffer(GL_ARRAY_BUFFER, m_vbo);

		gl33::BufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(m_verts.size() * sizeof(Vtx)), m_verts.data(), GL_DYNAMIC_DRAW);
		gl33::DrawArrays(GL_TRIANGLES, 0, (GLsizei)m_verts.size());

		gl33::BindVertexArray(0);
	}

} // namespace dd
