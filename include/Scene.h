#ifndef __SCENE_H__
#define __SCENE_H__

#include "pch.hpp"
#include "BasicMesh.h"
#include <glm/glm.hpp>


class Scene {
protected:
	GLuint _cubeShader;
	BasicMesh _cube;
	
	glm::mat4 _proj, _view, _model;
	
public:
	bool init();
	void update();
	void render();
	void release();
	void handleEvents(const SDL_Event& e);
};

#endif