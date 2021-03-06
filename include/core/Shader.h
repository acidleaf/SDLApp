#ifndef __SHADER_H__
#define __SHADER_H__

#include "pch.h"

namespace Shader {
	GLuint compileShaderSrc(const char* shaderSrc, GLenum shaderType);
	GLuint createProgramSrc(const char* vshSrc, const char* fshSrc);
};

#endif