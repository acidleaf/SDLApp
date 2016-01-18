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
uniform bool _grayscale;

void main() {
	vec3 color = texture(_tex, f_uv).rgb;
	frag_color = vec4(_grayscale ? color.rrr : color.rgb, _alpha);
}
)shader_src";



GLuint Surface::_shaderProg = 0;


bool Surface::init(const Tex2D& tex) {
	
	if (tex.id == 0) return false;
	if (tex.width == 0 && tex.height == 0) return false;
	
	_texID = tex.id;
	_size = { tex.width, tex.height };
	_grayscale = tex.format == GL_RED;
	
	
	// x, y, tx, ty
	const GLfloat verts[] = {
		0.0f, 0.0f, 0.0f, 0.0f,
		_size.x, 0.0f, 1.0f, 0.0f,
		_size.x, _size.y, 1.0f, 1.0f,
		0.0f, _size.y, 0.0f, 1.0f
	};
	
	
	const GLushort indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	

	if (!_mesh.init(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT)) return false;
	_mesh.bufferData(verts, sizeof(verts), indices, sizeof(indices), GL_STATIC_DRAW);
	_mesh.vertexAttrib(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	_mesh.vertexAttrib(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 8);
	
	
	
	if (!_shaderProg) {
		_shaderProg = Shader::createProgramSrc(surfaceVsh, surfaceFsh);
		if (!_shaderProg) return false;
	}
	
	auto app = App::getInstance();
	
	// Setup MVP matrix
	_proj = glm::ortho(
		0.0f, (float)(app->w()),
		(float)(app->h()), 0.0f,
		0.0f, 1.0f
	);
	_view = glm::lookAt(
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	_model = glm::translate(glm::mat4(1.0f), glm::vec3(_pos, 0));
	
	return true;
}


void Surface::render() {
	auto app = App::getInstance();
	_proj = glm::ortho(
		0.0f, (float)(app->w()),
		(float)(app->h()), 0.0f,
		0.0f, 1.0f
	);

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
	
	uloc = glGetUniformLocation(_shaderProg, "_grayscale");
	glUniform1i(uloc, _grayscale);
	
	_mesh.render();
}

void Surface::release() {
	_mesh.release();
}


void Surface::setPos(int x, int y) {
	_pos = {x, y};
	_model = glm::translate(glm::mat4(1.0f), glm::vec3(_pos, 0));
	_model = glm::scale(_model, glm::vec3(_scale, _scale, 1));
}

void Surface::setScale(float scale) {
	_scale = scale;
	_model = glm::translate(glm::mat4(1.0f), glm::vec3(_pos, 0));
	_model = glm::scale(_model, glm::vec3(_scale, _scale, 1));
}