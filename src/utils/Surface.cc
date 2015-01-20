#include "Surface.h"
#include "App.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>


const char* surfaceVsh = R"shader_src(
#version 330 core

layout(location = 0) in vec2 v_pos;
layout(location = 1) in vec2 v_texCoord;

out vec2 f_uv;

uniform mat4 _mvp;

void main() {
	gl_Position = _mvp * vec4(v_pos, 0, 1);
	f_uv = v_texCoord;
}
)shader_src";

const char* surfaceFsh = R"shader_src(
#version 330 core
precision mediump float;

in vec2 f_uv;
out vec4 frag_color;

uniform sampler2D _tex;
uniform float _alpha;

void main() {
	frag_color = vec4(texture(_tex, f_uv).rgb, _alpha);
}
)shader_src";



GLuint Surface::_shaderProg = 0;


bool Surface::init(GLuint texID, GLuint width, GLuint height) {
	
	if (texID == 0) return false;
	if (width == 0 && height == 0) return false;
	
	_texID = texID;
	_width = width;
	_height = height;
	
	
	// x, y, tx, ty
	const GLfloat verts[] = {
		0.0f, 0.0f, 0.0f, 0.0f,
		(GLfloat)_width, 0.0f, 1.0f, 0.0f,
		(GLfloat)_width, (GLfloat)_height, 1.0f, 1.0f,
		0.0f, (GLfloat)_height, 0.0f, 1.0f
	};
	
	
	const GLushort indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	
	if (!_surface.init(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT)) return false;
	_surface.bufferData(verts, sizeof(verts), indices, sizeof(indices), GL_STATIC_DRAW);
	_surface.vertexAttrib(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	_surface.vertexAttrib(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 8);
	
	
	
	if (!_shaderProg) {
		_shaderProg = Shader::createProgramSrc(surfaceVsh, surfaceFsh);
		if (!_shaderProg) return false;
	}
	
	auto app = App::getInstance();
	
	// Setup MVP matrix
	_proj = glm::ortho(
		0.0f, (GLfloat)(app->resX()),
		(GLfloat)(app->resY()), 0.0f,
		0.0f, 1.0f
	);
	_view = glm::lookAt(
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	_model = glm::translate(glm::mat4(1.0f), glm::vec3(_x, _y, 0));
	
	return true;
}

void Surface::update() {
	
}

void Surface::render() {
	glUseProgram(_shaderProg);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texID);
	
	glm::mat4 mvp = _proj * _view * _model;
	GLuint uloc = glGetUniformLocation(_shaderProg, "_mvp");
	glUniformMatrix4fv(uloc, 1, GL_FALSE, &mvp[0][0]);
	
	uloc = glGetUniformLocation(_shaderProg, "_tex");
	glUniform1i(uloc, 0);
	
	uloc = glGetUniformLocation(_shaderProg, "_alpha");
	glUniform1f(uloc, _alpha);
	
	_surface.render();
}

void Surface::release() {
	_surface.release();
}


void Surface::setPos(GLint x, GLint y) {
	_x = x;
	_y = y;
	
	_model = glm::scale(glm::mat4(1.0f), glm::vec3(_scale, _scale, 1));
	_model = glm::translate(_model, glm::vec3(_x, _y, 0));
}

void Surface::setScale(GLfloat scale) {
	_scale = scale;
	
	_model = glm::scale(glm::mat4(1.0f), glm::vec3(_scale, _scale, 1));
	_model = glm::translate(_model, glm::vec3(_x, _y, 0));
}