#include "Scene.h"
#include "App.h"
#include "Shader.h"
#include "ShaderSrc.h"

// Define your own vertex format
struct Vert {
	GLfloat x, y;
	uint8_t r, g, b;
};


bool Scene::init() {
	auto app = App::getInstance();
	
	const Vert verts[] = {
		{10.0f, 10.0f, 0xff, 0x00, 0x00},
		{app->resX() - 10.0f, 10.0f, 0x00, 0xff, 0x00},
		{app->resX() - 10.0f, app->resY() - 10.0f, 0x00, 0x00, 0xff},
		{10.0f, app->resY() - 10.0f, 0xff, 0xff, 0xff},
	};
	
	const GLushort indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	
	
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	
	
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	
	
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	// Vertex pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), BUFFER_OFFSET(0));
	
	// Vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vert), BUFFER_OFFSET(8));
	
	
	_shaderID = Shader::createProgramSrc(ShaderSrc::basicVsh, ShaderSrc::basicFsh);
	if (!_shaderID) return false;
	
	
	// Setup MVP matrix
	glm::mat4 projection = glm::ortho(
		0.0f, (GLfloat)(app->resX()),
		(GLfloat)(app->resY()), 0.0f,
		0.0f, 1.0f
	);
	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	glm::mat4 model = glm::mat4(1.0f);
	
	_mvp = projection * view * model;
	
	
	return true;
}

void Scene::release() {
	glDeleteBuffers(1, &_ibo);
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);
	glDeleteProgram(_shaderID);
}

void Scene::update() {
	
}

void Scene::render() {
	glUseProgram(_shaderID);
	
	GLuint mvpLoc = glGetUniformLocation(_shaderID, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &_mvp[0][0]);
	
	glBindVertexArray(_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
	
	glBindVertexArray(0);
}