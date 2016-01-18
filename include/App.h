#ifndef __APP_H__
#define __APP_H__

#include "pch.h"
#include "GUI.h"
#include "nanovg.h"
#include "SceneManager.h"

class App {
protected:
	SDL_Window* _window = nullptr;
	SDL_GLContext _ctx;
	SDL_Texture* _tex;
	
	bool _done = false;
	
	int _w = 0, _h = 0;
	int argc = 0;
	const char** argv = nullptr;
	
	Gui _gui;
	NVGcontext* _vg = nullptr;
	
	SceneManager _scene;

	void initGL();
	void update();
	void render();
	void handleEvents();
	
public:
	bool init(const char* title, int resX, int resY, int argc, const char** argv);
	void release();
	
	void run();
	void exit() { _done = true; }
	
	static App*& getInstance();
	
	int w() const { return _w; }
	int h() const { return _h; }
	
	
	NVGcontext* vg() { return _vg; }
};

#endif