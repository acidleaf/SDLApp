#ifndef __SDLAPP_H__
#define __SDLAPP_H__


class SDLApp {
private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	
	int m_xRes;
	int m_yRes;
	
	bool m_done;
	bool m_fullscreen;
	
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

inline bool SDLApp::done() const { return m_done; }
inline SDL_Window* SDLApp::window() { return m_window; }
inline SDL_GLContext& SDLApp::context() { return m_glContext; }


#endif
