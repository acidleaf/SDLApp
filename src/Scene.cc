#include "Scene.h"
#include "App.h"
#include "Shader.h"
#include "ShaderSrc.h"
#include <glm/gtc/matrix_transform.hpp>

bool Scene::init() {
	auto app = App::getInstance();
	
	GLfloat hw = 1.0f, hh = 1.0f, hd = 1.0f;
	const GLfloat verts[] = {
		-hw,  hh,  hd,
		-hw, -hh,  hd,
		 hw,  hh,  hd,
		 hw, -hh,  hd,
		 hw,  hh, -hd,
		 hw, -hh, -hd,
		-hw,  hh, -hd,
		-hw, -hh, -hd
	};
	const GLushort indices[] = {
		// Front
		0, 1, 2, 1, 3, 2,
		// Right
		2, 3, 4, 3, 5, 4,
		// Back
		4, 5, 6, 5, 7, 6,
		// Left
		6, 7, 0, 7, 1, 0,
		// Top
		6, 0, 4, 0, 2, 4,
		// Bottom
		1, 7, 3, 7, 5, 3
	};
	
	
	if (!_cube.init(GL_TRIANGLES, sizeof(indices) / sizeof(GLushort), GL_UNSIGNED_SHORT)) return false;
	_cube.bufferData(verts, sizeof(verts), indices, sizeof(indices), GL_STATIC_DRAW);
	_cube.vertexAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
	
	
	
	_cubeShader = Shader::createProgramSrc(ShaderSrc::cubeVsh, ShaderSrc::cubeFsh);
	if (!_cubeShader) return false;
	
	
	// Setup MVP matrix
	_proj = glm::perspective(45.0f, (GLfloat)(app->resX()) / (GLfloat)(app->resY()), 0.1f, 1000.0f);
	_view = glm::lookAt(
		glm::vec3(0, 0, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	_model = glm::mat4(1.0f);
	
	
	return true;
}

void Scene::release() {
	glDeleteProgram(_cubeShader);
	_cube.release();
}

void Scene::update() {
	_model = glm::rotate(_model, 1.0f, glm::vec3(0, 1, 0));
}

void Scene::render() {
	glUseProgram(_cubeShader);
	
	// Matrices
	GLuint uLoc = glGetUniformLocation(_cubeShader, "_view");
	glUniformMatrix4fv(uLoc, 1, GL_FALSE, &_view[0][0]);
	uLoc = glGetUniformLocation(_cubeShader, "_proj");
	glUniformMatrix4fv(uLoc, 1, GL_FALSE, &_proj[0][0]);
	uLoc = glGetUniformLocation(_cubeShader, "_model");
	glUniformMatrix4fv(uLoc, 1, GL_FALSE, &_model[0][0]);
	
	
	
	_cube.render();
}