#ifndef __SURFACE_H__
#define __SURFACE_H__

#include "pch.h"
#include "BasicMesh.h"
#include "Texture.h"

class Surface {
protected:
	static GLuint _shaderProg;
	
	glm::vec2 _pos;
	glm::vec2 _size;
	float _scale = 1.0f;
	float _alpha = 1.0f;
	bool _grayscale = false;
	
	
	GLuint _texID;
	BasicMesh _mesh;
	
	glm::mat4 _proj, _view, _model;
	
public:
	bool init(const Tex2D& tex);
	void render();
	void release();
	
	void setPos(int x, int y);
	void setScale(float scale);
	void setAlpha(float alpha) { _alpha = alpha; }
	
	glm::vec2 pos() const { return _pos; }
	glm::vec2 size() const { return _size; }
	
	float alpha() const { return _alpha; }
	float scale() const { return _scale; }
};

#endif