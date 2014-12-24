#ifndef __SHADER_H__
#define __SHADER_H__

#include "pch.hpp"
#include <string>

namespace Shader {
	GLuint compileShader(const std::string& shaderFilename, GLenum shaderType);
	GLuint createProgram(const std::string& vertexFileName, const std::string& fragmentFileName);
	
	GLuint compileShaderSrc(const char* shaderSrc, GLenum shaderType);
	GLuint createProgramSrc(const char* vshSrc, const char* fshSrc);
};

#endif