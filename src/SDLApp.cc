#include "SDLApp.h"

SDLApp::SDLApp() : m_done(0) {
}

SDLApp::~SDLApp() {
}

bool SDLApp::init(const char* title, int width, int height) {
	
	// Set OpenGL parameters to speed up drawing
	// RGB is 565 by default, override here
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	
	/*
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//*/
	
	
	
	m_xRes = width;
	m_yRes = height;
	
	// Init window
	m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if (!m_window) {
		printf("%s\n", SDL_GetError());
		return false;
	}
	
	// Init context
	m_glContext = SDL_GL_CreateContext(m_window);
	
	// Print GL version strings
	printf("OpenGL version: %s\n", glGetString(GL_VERSION));
	printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	SDL_GetWindowSize(m_window, &m_xRes, &m_yRes);
	
	// Initialize OpenGL states
	initGL();
	
	
	return true;
}

void SDLApp::release() {
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
}


void SDLApp::initGL() {
	
	glViewport(0, 0, m_xRes, m_yRes);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
}


void SDLApp::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) m_done = true;
		
	}
}

void SDLApp::update() {
}

void SDLApp::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	
	SDL_GL_SwapWindow(m_window);
}


