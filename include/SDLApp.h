#ifndef __SDLAPP_H__
#define __SDLAPP_H__


class SDLApp {
private:
	SDL_Window* _window;
	SDL_GLContext _glContext;
	
	int _xRes, _yRes;
	
	bool _done = false;
	
	void initGL();
	
public:
	SDLApp();
	~SDLApp();
	
	bool init(const char* title, int width, int height);
	void release();
	
	void handleEvents();
	void update();
	void render();
	
	
	bool done() const;
	SDL_Window* window();
	SDL_GLContext& context();
};

inline bool SDLApp::done() const { return _done; }
inline SDL_Window* SDLApp::window() { return _window; }
inline SDL_GLContext& SDLApp::context() { return _glContext; }


#endif
