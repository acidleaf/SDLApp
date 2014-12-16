#ifndef __SCENE_H__
#define __SCENE_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Scene {
protected:
	GLuint _shaderID;
	GLuint _vao, _vbo, _ibo;
	
	glm::mat4 _mvp;
	
public:
	bool init();
	void update();
	void render();
	void release();
};

#endif