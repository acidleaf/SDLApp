#include "Shader.h"

#include <vector>

GLuint Shader::compileShaderSrc(const char* shaderSrc, GLenum shaderType) {
	GLuint shaderID = glCreateShader(shaderType);
	GLint compilationSuccess = GL_FALSE;
	int infoLogLength;
	
	glShaderSource(shaderID, 1, &shaderSrc, nullptr);
	glCompileShader(shaderID);
	
	// Check shader
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationSuccess);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (!compilationSuccess) {
		std::vector<char> errorMsg(infoLogLength + 1);
		glGetShaderInfoLog(shaderID, infoLogLength, nullptr, &errorMsg[0]);
		printf("Error compiling shader: %s\n", &errorMsg[0]);
		return 0;
	}
	
	return shaderID;
}


GLuint Shader::createProgramSrc(const char* vshSrc, const char* fshSrc) {
	// Compile shaders
	GLuint vsh = compileShaderSrc(vshSrc, GL_VERTEX_SHADER);
	GLuint fsh = compileShaderSrc(fshSrc, GL_FRAGMENT_SHADER);
	
	// Link the program
	GLuint progID = glCreateProgram();
	glAttachShader(progID, vsh);
	glAttachShader(progID, fsh);
	glLinkProgram(progID);
	
	GLint compilationSuccess = GL_FALSE;
	int infoLogLength;
	glGetProgramiv(progID, GL_LINK_STATUS, &compilationSuccess);
	glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	if (!compilationSuccess) {
		std::vector<char> errorMsg(infoLogLength + 1);
		glGetProgramInfoLog(progID, infoLogLength, nullptr, &errorMsg[0]);
		printf("Error compiling shader: %s\n", &errorMsg[0]);
		return 0;
	}
	
	glDeleteShader(vsh);
	glDeleteShader(fsh);

	return progID;
}