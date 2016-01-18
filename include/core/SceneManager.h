#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "pch.h"

class IScene;
class SceneManager {
protected:
	IScene* _active = nullptr;
	
public:
	bool init();
	void release();
	
	void update();
	void render();
	
	bool handleEvents(const SDL_Event& e);
};


#endif