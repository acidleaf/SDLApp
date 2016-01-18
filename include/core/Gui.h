#ifndef __GUI_H__
#define __GUI_H__

#include "pch.h"
#include "imgui.h"

class Gui {
protected:
	SDL_Window* _window = NULL;
	double _time = 0.0f;
	
	bool _mouseState[3];
	float _mouseWheel = 0.0f;
	
	GLuint _fontTex = 0;
	GLuint _shaderProg = 0;
	GLuint _vbo = 0, _vao = 0, _ibo = 0;
	
	bool initGL();
	void releaseGL();
	void createFontTex();
	
public:
	bool init(SDL_Window* window, const char* ini = nullptr, const char* log = nullptr);
	void release();
	
	void update();
	void render();
	bool handleEvents(const SDL_Event& e);
};

#endif