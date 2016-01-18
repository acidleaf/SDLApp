#ifndef __BASICMESH_H__
#define __BASICMESH_H__

/*
	A wrapper for creating a simple OpenGL mesh object with custom vertex attributes and vertex data
	Basically used to store the states for draw calls
	- Uses interleaved vertex attributes
	- Only does indexed drawing
	- Only uses one index buffer and one vertex buffer
*/

#include "pch.h"

class BasicMesh {
protected:
	GLenum _drawMode, _elementType;
	GLsizei _numElements;
	GLuint _vao, _vbo, _ibo;
	
public:
	bool init(GLenum drawMode, GLsizei numElements, GLenum elementType);
	void release();
	
	void bufferData(const GLvoid* vertData, GLsizeiptr vertSize, const GLvoid* indexData, GLsizeiptr indexSize, GLenum usage);
	void vertexAttrib(GLuint index, GLint numComponents, GLenum type, GLboolean normalized, GLsizei stride, GLuint offset);
 	
	void render();
};

#endif