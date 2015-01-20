#include "App.h"


static App* _appInstance = nullptr;

App*& App::getInstance() {
	if (!_appInstance) _appInstance = new App();
	return _appInstance;
}


App::App() {}

App::~App() {}

bool App::init(const char* title, int width, int height) {
	
	_resX = width;
	_resY = height;
	
	
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL\n");
		return -1;
	}
	
	// Set OpenGL parameters to speed up drawing
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	// Init window
	_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if (!_window) {
		printf("%s\n", SDL_GetError());
		return false;
	}
	
	// Init context
	_glContext = SDL_GL_CreateContext(_window);
	SDL_GL_MakeCurrent(_window, _glContext);
	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		printf("Failed to init GLEW\n");
		return false;
	}
	
	// Print GL version strings
	printf("OpenGL version: %s\n", glGetString(GL_VERSION));
	printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	// Initialize OpenGL states
	initGL();
	
	
	
	// Initialize your stuff here
	if (!_gui.init()) return false;
	
	if (!_scene.init()) return false;
	
	
	
	
	return true;
}

void App::release() {
	_scene.release();
	
	_gui.release();
	
	SDL_GL_DeleteContext(_glContext);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}


void App::initGL() {
	glEnable(GL_TEXTURE_2D);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	
}


void App::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) _done = true;
		
		// Quit when ESC key is pressed
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) _done = true;
		
		_gui.handleEvents(e);
		
		if (!ImGui::GetIO().WantCaptureKeyboard && !ImGui::GetIO().WantCaptureMouse) {
			// Your event handling code here
		}
	}
}



void App::update() {
	_gui.update();
	_scene.update();
}

void App::render() {
	glViewport(0, 0, _resX, _resY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	_scene.render();
	
	_gui.render();
	
	SDL_GL_SwapWindow(_window);
}


