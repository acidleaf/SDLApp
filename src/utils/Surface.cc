#include "Surface.h"
#include "App.h"
#include "Shader.h"
#include "ShaderSrc.h"

struct SurfaceVert {
	GLfloat x, y;
	GLfloat tx, ty;
};

GLuint Surface::_shaderProg = 0;

bool Surface::init(GLuint texID, GLuint width, GLuint height) {
	
	if (texID == 0) return false;
	if (width == 0 && height == 0) return false;
	
	_texID = texID;
	_width = width;
	_height = height;
	
	const SurfaceVert verts[] = {
		{0.0f, 0.0f, 0.0f, 0.0f},
		{(GLfloat)_width, 0.0f, 1.0f, 0.0f},
		{(GLfloat)_width, (GLfloat)_height, 1.0f, 1.0f},
		{0.0f, (GLfloat)_height, 0.0f, 1.0f}
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
	// Vertices - stream 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SurfaceVert), BUFFER_OFFSET(0));
	
	// TexCoords - stream 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SurfaceVert), BUFFER_OFFSET(8));
	
	
	
	if (!_shaderProg) {
		_shaderProg = Shader::createProgramSrc(ShaderSrc::surfaceVsh, ShaderSrc::surfaceFsh);
		if (!_shaderProg) return false;
	}
	
	auto app = App::getInstance();
	
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
	model = glm::translate(model, glm::vec3(_x, _y, 0));
	
	
	_mvp = projection * view * model;
	
	
	
	return true;
}

void Surface::update() {
	
}

void Surface::render() {
	glUseProgram(_shaderProg);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texID);
	
	GLuint mvpLoc = glGetUniformLocation(_shaderProg, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &_mvp[0][0]);
	
	GLuint texLoc = glGetUniformLocation(_shaderProg, "tex");
	glUniform1i(texLoc, 0);
	
	GLuint alphaLoc = glGetUniformLocation(_shaderProg, "alpha");
	glUniform1f(alphaLoc, _alpha);
	
	glBindVertexArray(_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
	
	glBindVertexArray(0);
}

void Surface::release() {
	glDeleteBuffers(1, &_ibo);
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);
}


void Surface::setPos(GLint x, GLint y) {
	_x = x;
	_y = y;
	
	
	auto app = App::getInstance();
	
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
	model = glm::scale(model, glm::vec3(_scale, _scale, 1));
	model = glm::translate(model, glm::vec3(_x, _y, 0));
	
	
	_mvp = projection * view * model;
}

void Surface::setScale(GLfloat scale) {
	_scale = scale;
	
	auto app = App::getInstance();
	
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
	model = glm::scale(model, glm::vec3(_scale, _scale, 1));
	model = glm::translate(model, glm::vec3(_x, _y, 0));
	
	
	_mvp = projection * view * model;
}