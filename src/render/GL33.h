#pragma once
#include <SDL.h>
#include <cstdint>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using GLenum = unsigned int;
using GLboolean = unsigned char;
using GLint = int;
using GLsizei = int;
using GLuint = unsigned int;
using GLfloat = float;
using GLchar = char;
using GLbitfield = unsigned int;
using GLintptr = std::intptr_t;
using GLsizeiptr = std::intptr_t;

#ifndef GL_FALSE
#define GL_FALSE 0
#endif

#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_DYNAMIC_DRAW
#define GL_DYNAMIC_DRAW 0x88E8
#endif
#ifndef GL_FLOAT
#define GL_FLOAT 0x1406
#endif
#ifndef GL_TRIANGLES
#define GL_TRIANGLES 0x0004
#endif

#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif

#ifndef GL_TEXTURE_2D
#define GL_TEXTURE_2D 0x0DE1
#endif
#ifndef GL_RGBA
#define GL_RGBA 0x1908
#endif
#ifndef GL_UNSIGNED_BYTE
#define GL_UNSIGNED_BYTE 0x1401
#endif
#ifndef GL_TEXTURE0
#define GL_TEXTURE0 0x84C0
#endif
#ifndef GL_TEXTURE_MIN_FILTER
#define GL_TEXTURE_MIN_FILTER 0x2801
#endif
#ifndef GL_TEXTURE_MAG_FILTER
#define GL_TEXTURE_MAG_FILTER 0x2800
#endif
#ifndef GL_NEAREST
#define GL_NEAREST 0x2600
#endif
#ifndef GL_TEXTURE_WRAP_S
#define GL_TEXTURE_WRAP_S 0x2802
#endif
#ifndef GL_TEXTURE_WRAP_T
#define GL_TEXTURE_WRAP_T 0x2803
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#ifndef GL_BLEND
#define GL_BLEND 0x0BE2
#endif
#ifndef GL_SRC_ALPHA
#define GL_SRC_ALPHA 0x0302
#endif
#ifndef GL_ONE_MINUS_SRC_ALPHA
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#endif

#ifndef GL_COLOR_BUFFER_BIT
#define GL_COLOR_BUFFER_BIT 0x00004000
#endif
#ifndef GL_DEPTH_TEST
#define GL_DEPTH_TEST 0x0B71
#endif

namespace dd::gl33 {

	using PFN_glViewport = void (*)(GLint, GLint, GLsizei, GLsizei);
	using PFN_glClearColor = void (*)(GLfloat, GLfloat, GLfloat, GLfloat);
	using PFN_glClear = void (*)(GLbitfield);
	using PFN_glEnable = void (*)(GLenum);
	using PFN_glDisable = void (*)(GLenum);

	using PFN_glCreateShader = GLuint(*)(GLenum);
	using PFN_glShaderSource = void (*)(GLuint, GLsizei, const GLchar* const*, const GLint*);
	using PFN_glCompileShader = void (*)(GLuint);
	using PFN_glGetShaderiv = void (*)(GLuint, GLenum, GLint*);
	using PFN_glGetShaderInfoLog = void (*)(GLuint, GLsizei, GLsizei*, GLchar*);
	using PFN_glCreateProgram = GLuint(*)();
	using PFN_glAttachShader = void (*)(GLuint, GLuint);
	using PFN_glLinkProgram = void (*)(GLuint);
	using PFN_glGetProgramiv = void (*)(GLuint, GLenum, GLint*);
	using PFN_glGetProgramInfoLog = void (*)(GLuint, GLsizei, GLsizei*, GLchar*);
	using PFN_glUseProgram = void (*)(GLuint);
	using PFN_glDeleteShader = void (*)(GLuint);
	using PFN_glDeleteProgram = void (*)(GLuint);

	using PFN_glGenVertexArrays = void (*)(GLsizei, GLuint*);
	using PFN_glBindVertexArray = void (*)(GLuint);
	using PFN_glGenBuffers = void (*)(GLsizei, GLuint*);
	using PFN_glBindBuffer = void (*)(GLenum, GLuint);
	using PFN_glBufferData = void (*)(GLenum, GLsizeiptr, const void*, GLenum);
	using PFN_glBufferSubData = void (*)(GLenum, GLintptr, GLsizeiptr, const void*);
	using PFN_glEnableVertexAttribArray = void (*)(GLuint);
	using PFN_glVertexAttribPointer = void (*)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
	using PFN_glDeleteBuffers = void (*)(GLsizei, const GLuint*);
	using PFN_glDeleteVertexArrays = void (*)(GLsizei, const GLuint*);

	using PFN_glGetUniformLocation = GLint(*)(GLuint, const GLchar*);
	using PFN_glUniform1i = void (*)(GLint, GLint);
	using PFN_glUniformMatrix4fv = void (*)(GLint, GLsizei, GLboolean, const GLfloat*);

	using PFN_glActiveTexture = void (*)(GLenum);
	using PFN_glGenTextures = void (*)(GLsizei, GLuint*);
	using PFN_glBindTexture = void (*)(GLenum, GLuint);
	using PFN_glTexParameteri = void (*)(GLenum, GLenum, GLint);
	using PFN_glTexImage2D = void (*)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*);
	using PFN_glDeleteTextures = void (*)(GLsizei, const GLuint*);

	using PFN_glBlendFunc = void (*)(GLenum, GLenum);

	using PFN_glDrawArrays = void (*)(GLenum, GLint, GLsizei);

	extern PFN_glViewport   Viewport;
	extern PFN_glClearColor ClearColor;
	extern PFN_glClear      Clear;
	extern PFN_glEnable     Enable;
	extern PFN_glDisable    Disable;
	extern PFN_glBlendFunc  BlendFunc;

	extern PFN_glCreateShader      CreateShader;
	extern PFN_glShaderSource      ShaderSource;
	extern PFN_glCompileShader     CompileShader;
	extern PFN_glGetShaderiv       GetShaderiv;
	extern PFN_glGetShaderInfoLog  GetShaderInfoLog;
	extern PFN_glCreateProgram     CreateProgram;
	extern PFN_glAttachShader      AttachShader;
	extern PFN_glLinkProgram       LinkProgram;
	extern PFN_glGetProgramiv      GetProgramiv;
	extern PFN_glGetProgramInfoLog GetProgramInfoLog;
	extern PFN_glUseProgram        UseProgram;
	extern PFN_glDeleteShader      DeleteShader;
	extern PFN_glDeleteProgram     DeleteProgram;

	extern PFN_glGenVertexArrays         GenVertexArrays;
	extern PFN_glBindVertexArray         BindVertexArray;
	extern PFN_glGenBuffers              GenBuffers;
	extern PFN_glBindBuffer              BindBuffer;
	extern PFN_glBufferData              BufferData;
	extern PFN_glBufferSubData           BufferSubData;
	extern PFN_glEnableVertexAttribArray EnableVertexAttribArray;
	extern PFN_glVertexAttribPointer     VertexAttribPointer;
	extern PFN_glDeleteBuffers           DeleteBuffers;
	extern PFN_glDeleteVertexArrays      DeleteVertexArrays;

	extern PFN_glGetUniformLocation GetUniformLocation;
	extern PFN_glUniform1i          Uniform1i;
	extern PFN_glUniformMatrix4fv   UniformMatrix4fv;

	extern PFN_glActiveTexture  ActiveTexture;
	extern PFN_glGenTextures    GenTextures;
	extern PFN_glBindTexture    BindTexture;
	extern PFN_glTexParameteri  TexParameteri;
	extern PFN_glTexImage2D     TexImage2D;
	extern PFN_glDeleteTextures DeleteTextures;

	extern PFN_glDrawArrays DrawArrays;

	bool Load();

} // namespace dd::gl33
