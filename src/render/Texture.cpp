#include "render/Texture.h"
#include "render/GL33.h"

namespace dd {

	void Texture::CreateWhite() {
		gl33::GenTextures(1, &m_id);
		gl33::BindTexture(GL_TEXTURE_2D, m_id);

		const unsigned char pixel[4] = { 255, 255, 255, 255 };
		gl33::TexImage2D(GL_TEXTURE_2D, 0, (int)GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

		gl33::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_NEAREST);
		gl33::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_NEAREST);
		gl33::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)GL_CLAMP_TO_EDGE);
		gl33::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)GL_CLAMP_TO_EDGE);
	}

	void Texture::Bind(int unit) const {
		gl33::ActiveTexture((GLenum)((int)GL_TEXTURE0 + unit));
		gl33::BindTexture(GL_TEXTURE_2D, m_id);
	}

} // namespace dd
