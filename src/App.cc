#include "App.h"

#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"


static App* _appInstance = nullptr;
App*& App::getInstance() {
	if (!_appInstance) _appInstance = new App();
	return _appInstance;
}

bool App::init(const char* title, int resX, int resY, int argc, const char** argv) {
	_w = resX;
	_h = resY;
	
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}
	
	// Init window
	uint32_t windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;
	//windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;
	_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _w, _h, windowFlags);
	if (!_window) {
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}
	
	// Init context
	// Set OpenGL parameters to speed up drawing
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	_ctx = SDL_GL_CreateContext(_window);
	SDL_GL_MakeCurrent(_window, _ctx);
	
	SDL_GL_SetSwapInterval(1);
	
	
	// Init GLAD
	if (!gladLoadGL()) {
		SDL_Log("Failed to init GLAD\n");
		return false;
	}
	
	initGL();
	
	// Print SDL version
	SDL_version v;
	SDL_GetVersion(&v);
	SDL_Log("SDL version: %d.%d.%d\n", v.major, v.minor, v.patch);
	
	// Print GL version strings
	SDL_Log("OpenGL version: %s\n", glGetString(GL_VERSION));
	SDL_Log("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	
	// Init nanovg
	_vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
	
	// Init Gui
	if (!_gui.init(_window, "data/ui.ini")) return false;

	// Init scenes
	if (!_scene.init()) return false;

	
	return true;
}

void App::initGL() {
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.14f, 0.14f, 0.14f, 1.0f);
}

void App::release() {
	// Release app specific
	_scene.release();


	// Release platform specific
	_gui.release();
	nvgDeleteGL3(_vg);
	
	
	SDL_GL_DeleteContext(_ctx);
	SDL_DestroyWindow(_window);
	SDL_Quit();
	
	delete _appInstance;
}

void App::run() {
	while (!_done) {
		handleEvents();
		update();
		render();
	}
}

void App::update() {
	// Update screen resolution
	SDL_GetWindowSize(_window, &_w, &_h);

	_gui.update();
	_scene.update();
}

void App::render() {
	glViewport(0, 0, _w, _h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	nvgBeginFrame(_vg, _w, _h, (float)_w / _h);

	_scene.render();
	
	nvgEndFrame(_vg);
	
	_gui.render();


	SDL_GL_SwapWindow(_window);
}

void App::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		
		if (e.type == SDL_QUIT) _done = true;
		if (e.type == SDL_KEYDOWN) {
			// Quit when ESC key is pressed
			if (e.key.keysym.sym == SDLK_ESCAPE) _done = true;
		}
		
		
		_gui.handleEvents(e);

		
		auto io = ImGui::GetIO();
		if (!io.WantCaptureMouse && !io.WantCaptureKeyboard) {
			_scene.handleEvents(e);
		}
		
		
	}
}