#ifndef __ISCENE_H__
#define __ISCENE_H__

#include "pch.h"

class IScene {
public:
	virtual ~IScene() {}
	
	virtual bool init() = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	
	virtual bool handleEvents(const SDL_Event& e) = 0;
};

#endif