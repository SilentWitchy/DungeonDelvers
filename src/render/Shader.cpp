#include "render/Shader.h"
#include "render/GL33.h"
#include "core/Log.h"

#include <fstream>
#include <sstream>
#include <string>

namespace dd {

	static std::string ReadAll(const char* path) {
		std::ifstream f(path, std::ios::binary);
		if (!f) return {};
		std::ostringstream ss;
		ss << f.rdbuf();
		return ss.str();
	}

	static bool CheckShader(unsigned int sh, const char* label) {
		int ok = 0;
		gl33::GetShaderiv(sh, GL_COMPILE_STATUS, &ok);
		if (ok) return true;

		char buf[2048]{};
		int n = 0;
		gl33::GetShaderInfoLog(sh, (GLsizei)sizeof(buf) - 1, (GLsizei*)&n, buf);
		LogError(std::string("Shader compile failed (") + label + "): " + buf);
		return false;
	}

	static bool CheckProgram(unsigned int prog) {
		int ok = 0;
		gl33::GetProgramiv(prog, GL_LINK_STATUS, &ok);
		if (ok) return true;

		char buf[2048]{};
		int n = 0;
		gl33::GetProgramInfoLog(prog, (GLsizei)sizeof(buf) - 1, (GLsizei*)&n, buf);
		LogError(std::string("Program link failed: ") + buf);
		return false;
	}

	bool Shader::LoadFromFiles(const char* vsPath, const char* fsPath) {
		const std::string vs = ReadAll(vsPath);
		const std::string fs = ReadAll(fsPath);

		if (vs.empty()) { LogError(std::string("Failed to read VS: ") + vsPath); return false; }
		if (fs.empty()) { LogError(std::string("Failed to read FS: ") + fsPath); return false; }

		const char* vsp = vs.c_str();
		const char* fsp = fs.c_str();

		const unsigned int vsh = gl33::CreateShader(GL_VERTEX_SHADER);
		gl33::ShaderSource(vsh, 1, &vsp, nullptr);
		gl33::CompileShader(vsh);
		if (!CheckShader(vsh, "vertex")) return false;

		const unsigned int fsh = gl33::CreateShader(GL_FRAGMENT_SHADER);
		gl33::ShaderSource(fsh, 1, &fsp, nullptr);
		gl33::CompileShader(fsh);
		if (!CheckShader(fsh, "fragment")) return false;

		m_prog = gl33::CreateProgram();
		gl33::AttachShader(m_prog, vsh);
		gl33::AttachShader(m_prog, fsh);
		gl33::LinkProgram(m_prog);

		gl33::DeleteShader(vsh);
		gl33::DeleteShader(fsh);

		if (!CheckProgram(m_prog)) return false;
		return true;
	}

	void Shader::Bind() const {
		gl33::UseProgram(m_prog);
	}

	void Shader::SetInt(const char* name, int v) const {
		const int loc = gl33::GetUniformLocation(m_prog, name);
		if (loc >= 0) gl33::Uniform1i(loc, v);
	}

	void Shader::SetMat4(const char* name, const Mat4& m) const {
		const int loc = gl33::GetUniformLocation(m_prog, name);
		if (loc >= 0) gl33::UniformMatrix4fv(loc, 1, GL_FALSE, m.m.data());
	}

} // namespace dd
