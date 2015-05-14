#ifndef __SURFACE_H__
#define __SURFACE_H__

#include "pch.hpp"
#include "BasicMesh.h"

class Surface {
protected:
	static GLuint _shaderProg;
	
	GLfloat _scale = 1.0f;
	GLuint _width = 0, _height = 0;
	GLint _x = 0, _y = 0;
	GLfloat _alpha = 1.0f;
	bool _grayscale = false;
	
	GLuint _texID;
	BasicMesh _surface;
	
	glm::mat4 _proj, _view, _model;
	
public:
	bool init(GLuint texID, GLuint width, GLuint height, bool grayscale = false);
	void update();
	void render();
	void release();
	
	void setPos(GLint x, GLint y);
	void setScale(GLfloat scale);
	void setAlpha(float alpha) { _alpha = alpha; }
	
	GLuint x() const { return _x; }
	GLuint y() const { return _y; }
	GLuint width() const { return _width; }
	GLuint height() const { return _height; }
	GLfloat alpha() const { return _alpha; }
	
	GLfloat scale() const { return _scale; }
};

#endif