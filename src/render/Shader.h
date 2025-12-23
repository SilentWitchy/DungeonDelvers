#pragma once
#include "render/Camera.h"

namespace dd {

	class Shader final {
	public:
		bool LoadFromFiles(const char* vsPath, const char* fsPath);

		void Bind() const;

		void SetInt(const char* name, int v) const;
		void SetMat4(const char* name, const Mat4& m) const;

	private:
		unsigned int m_prog = 0;
	};

} // namespace dd
