#ifndef __GUI_H__
#define __GUI_H__

#include "pch.hpp"
#include "imgui.h"


class Gui {
protected:
	GLuint _maxVBOSize = 1024;
	
	GLuint _vao, _vbo;
	GLuint _fontTexID;
	GLuint _guiShader = 0;
	GLuint _prevTime = 0;
	glm::mat4 _proj;
	
	bool initImGui();
	void imguiRender(ImDrawList** const lists, int count);
	
	friend void gui_RenderDrawLists(ImDrawList** const lists, int count);
	
public:
	bool init();
	void release();
	
	void update();
	void render();
	
	void handleEvents(const SDL_Event& e);
};


#endif