#include "render/GL33.h"
#include "core/Log.h"
#include <string>

namespace dd::gl33 {

    static void* LoadProc(const char* name) {
        void* p = SDL_GL_GetProcAddress(name);
        if (!p) dd::LogError(std::string("Missing GL proc: ") + name);
        return p;
    }

    PFN_glViewport   Viewport = nullptr;
    PFN_glClearColor ClearColor = nullptr;
    PFN_glClear      Clear = nullptr;
    PFN_glEnable     Enable = nullptr;
    PFN_glDisable    Disable = nullptr;
    PFN_glBlendFunc  BlendFunc = nullptr;

    PFN_glCreateShader      CreateShader = nullptr;
    PFN_glShaderSource      ShaderSource = nullptr;
    PFN_glCompileShader     CompileShader = nullptr;
    PFN_glGetShaderiv       GetShaderiv = nullptr;
    PFN_glGetShaderInfoLog  GetShaderInfoLog = nullptr;
    PFN_glCreateProgram     CreateProgram = nullptr;
    PFN_glAttachShader      AttachShader = nullptr;
    PFN_glLinkProgram       LinkProgram = nullptr;
    PFN_glGetProgramiv      GetProgramiv = nullptr;
    PFN_glGetProgramInfoLog GetProgramInfoLog = nullptr;
    PFN_glUseProgram        UseProgram = nullptr;
    PFN_glDeleteShader      DeleteShader = nullptr;
    PFN_glDeleteProgram     DeleteProgram = nullptr;

    PFN_glGenVertexArrays         GenVertexArrays = nullptr;
    PFN_glBindVertexArray         BindVertexArray = nullptr;
    PFN_glGenBuffers              GenBuffers = nullptr;
    PFN_glBindBuffer              BindBuffer = nullptr;
    PFN_glBufferData              BufferData = nullptr;
    PFN_glBufferSubData           BufferSubData = nullptr;
    PFN_glEnableVertexAttribArray EnableVertexAttribArray = nullptr;
    PFN_glVertexAttribPointer     VertexAttribPointer = nullptr;
    PFN_glDeleteBuffers           DeleteBuffers = nullptr;
    PFN_glDeleteVertexArrays      DeleteVertexArrays = nullptr;

    PFN_glGetUniformLocation GetUniformLocation = nullptr;
    PFN_glUniform1i          Uniform1i = nullptr;
    PFN_glUniformMatrix4fv   UniformMatrix4fv = nullptr;

    PFN_glActiveTexture  ActiveTexture = nullptr;
    PFN_glGenTextures    GenTextures = nullptr;
    PFN_glBindTexture    BindTexture = nullptr;
    PFN_glTexParameteri  TexParameteri = nullptr;
    PFN_glTexImage2D     TexImage2D = nullptr;
    PFN_glDeleteTextures DeleteTextures = nullptr;

    PFN_glDrawArrays DrawArrays = nullptr;

    bool Load() {
        Viewport = (PFN_glViewport)LoadProc("glViewport");
        ClearColor = (PFN_glClearColor)LoadProc("glClearColor");
        Clear = (PFN_glClear)LoadProc("glClear");
        Enable = (PFN_glEnable)LoadProc("glEnable");
        Disable = (PFN_glDisable)LoadProc("glDisable");
        BlendFunc = (PFN_glBlendFunc)LoadProc("glBlendFunc");

        CreateShader = (PFN_glCreateShader)LoadProc("glCreateShader");
        ShaderSource = (PFN_glShaderSource)LoadProc("glShaderSource");
        CompileShader = (PFN_glCompileShader)LoadProc("glCompileShader");
        GetShaderiv = (PFN_glGetShaderiv)LoadProc("glGetShaderiv");
        GetShaderInfoLog = (PFN_glGetShaderInfoLog)LoadProc("glGetShaderInfoLog");
        CreateProgram = (PFN_glCreateProgram)LoadProc("glCreateProgram");
        AttachShader = (PFN_glAttachShader)LoadProc("glAttachShader");
        LinkProgram = (PFN_glLinkProgram)LoadProc("glLinkProgram");
        GetProgramiv = (PFN_glGetProgramiv)LoadProc("glGetProgramiv");
        GetProgramInfoLog = (PFN_glGetProgramInfoLog)LoadProc("glGetProgramInfoLog");
        UseProgram = (PFN_glUseProgram)LoadProc("glUseProgram");
        DeleteShader = (PFN_glDeleteShader)LoadProc("glDeleteShader");
        DeleteProgram = (PFN_glDeleteProgram)LoadProc("glDeleteProgram");

        GenVertexArrays = (PFN_glGenVertexArrays)LoadProc("glGenVertexArrays");
        BindVertexArray = (PFN_glBindVertexArray)LoadProc("glBindVertexArray");
        GenBuffers = (PFN_glGenBuffers)LoadProc("glGenBuffers");
        BindBuffer = (PFN_glBindBuffer)LoadProc("glBindBuffer");
        BufferData = (PFN_glBufferData)LoadProc("glBufferData");
        BufferSubData = (PFN_glBufferSubData)LoadProc("glBufferSubData");
        EnableVertexAttribArray = (PFN_glEnableVertexAttribArray)LoadProc("glEnableVertexAttribArray");
        VertexAttribPointer = (PFN_glVertexAttribPointer)LoadProc("glVertexAttribPointer");
        DeleteBuffers = (PFN_glDeleteBuffers)LoadProc("glDeleteBuffers");
        DeleteVertexArrays = (PFN_glDeleteVertexArrays)LoadProc("glDeleteVertexArrays");

        GetUniformLocation = (PFN_glGetUniformLocation)LoadProc("glGetUniformLocation");
        Uniform1i = (PFN_glUniform1i)LoadProc("glUniform1i");
        UniformMatrix4fv = (PFN_glUniformMatrix4fv)LoadProc("glUniformMatrix4fv");

        ActiveTexture = (PFN_glActiveTexture)LoadProc("glActiveTexture");
        GenTextures = (PFN_glGenTextures)LoadProc("glGenTextures");
        BindTexture = (PFN_glBindTexture)LoadProc("glBindTexture");
        TexParameteri = (PFN_glTexParameteri)LoadProc("glTexParameteri");
        TexImage2D = (PFN_glTexImage2D)LoadProc("glTexImage2D");
        DeleteTextures = (PFN_glDeleteTextures)LoadProc("glDeleteTextures");

        DrawArrays = (PFN_glDrawArrays)LoadProc("glDrawArrays");

        const bool ok =
            Viewport && ClearColor && Clear && Enable && Disable && BlendFunc &&
            CreateShader && CreateProgram && UseProgram &&
            GenVertexArrays && GenBuffers && VertexAttribPointer &&
            GetUniformLocation && UniformMatrix4fv &&
            GenTextures && TexImage2D &&
            DrawArrays;

        if (!ok) dd::LogError("OpenGL function loading failed.");
        return ok;
    }

} // namespace dd::gl33
