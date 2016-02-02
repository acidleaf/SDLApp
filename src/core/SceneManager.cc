#include "SceneManager.h"
#include "IScene.h"

bool SceneManager::init() {
	memset(_scenes, 0, sizeof(IScene*) * MAX_SCENES);
	_numScenes = 0;
	
	return true;
}
void SceneManager::release() {
	for (int i = 0; i < _numScenes; ++i) {
		if (_scenes[i]) {
			_scenes[i]->release();
			delete _scenes[i];
		}
	}
}

void SceneManager::update() {
	if (_active) _active->update();
}
void SceneManager::render() {
	if (_active) _active->render();
}

bool SceneManager::handleEvents(const SDL_Event& e) {
	if (_active) return _active->handleEvents(e);
	return false;
}

IScene* SceneManager::addScene(const char* name, IScene* scene) {
	if (_numScenes >= MAX_SCENES) return nullptr;
	
	scene->name(name);
	auto r = _scenes[_numScenes] = scene;
	++_numScenes;
	return r;
}