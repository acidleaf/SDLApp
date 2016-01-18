#include "Shader.h"

GLuint Shader::compileShaderSrc(const char* shaderSrc, GLenum shaderType) {
	GLuint shaderID = glCreateShader(shaderType);
	GLint compilationSuccess = GL_FALSE;
	char errorMsg[128];
	int infoLogLength;
	
	glShaderSource(shaderID, 1, &shaderSrc, nullptr);
	glCompileShader(shaderID);
	
	// Check shader
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationSuccess);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (!compilationSuccess) {
		glGetShaderInfoLog(shaderID, 128, nullptr, errorMsg);
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
	char errorMsg[128];
	int infoLogLength;
	glGetProgramiv(progID, GL_LINK_STATUS, &compilationSuccess);
	glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	if (!compilationSuccess) {
		glGetProgramInfoLog(progID, 128, nullptr, errorMsg);
		printf("Error creating shader program: %s\n", errorMsg);
		return 0;
	}
	
	glDeleteShader(vsh);
	glDeleteShader(fsh);

	return progID;
}