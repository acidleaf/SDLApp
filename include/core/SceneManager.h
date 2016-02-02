#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "pch.h"

class IScene;
class SceneManager {
protected:
	static const int MAX_SCENES = 10;
	
	IScene* _scenes[MAX_SCENES];
	int _numScenes = 0;
	
	IScene* _active = nullptr;
	
	IScene* addScene(const char* name, IScene* scene);
	
public:
	bool init();
	void release();
	
	void update();
	void render();
	
	bool handleEvents(const SDL_Event& e);
};


#endif