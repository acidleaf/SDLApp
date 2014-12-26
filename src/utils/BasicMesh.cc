#include "BasicMesh.h"

bool BasicMesh::init(GLenum drawMode, GLsizei numElements, GLenum elementType) {
	_drawMode = drawMode;
	_numElements = numElements;
	_elementType = elementType;
	
	glGetError();
	
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);
	
	glBindVertexArray(0);
	
	return glGetError() == GL_NO_ERROR;
}


void BasicMesh::release() {
	glDeleteBuffers(1, &_ibo);
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);
}

void BasicMesh::bufferData(const GLvoid* vertData, GLsizeiptr vertSize, const GLvoid* indexData, GLsizeiptr indexSize, GLenum usage) {
	glBindVertexArray(_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertSize, vertData, usage);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indexData, usage);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void BasicMesh::vertexAttrib(GLuint index, GLint numComponents, GLenum type, GLboolean normalized, GLsizei stride, GLuint offset) {
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, numComponents, type, normalized, stride, BUFFER_OFFSET(offset));
	
	glBindVertexArray(0);
}

void BasicMesh::render() {
	glBindVertexArray(_vao);
	// Assumes we're drawing from the start of the index buffer
	glDrawElements(_drawMode, _numElements, _elementType, BUFFER_OFFSET(0));
	glBindVertexArray(0);
}