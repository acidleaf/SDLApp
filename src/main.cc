#include "SDLApp.h"


// A base SDL App that displays an empty window

int main(int argc,  char * argv[]) {
	
	SDLApp app;
	
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL\n");
		
		return 1;
	}
	
	app.init("SDLApp", 800, 600);
	
	
	while (!app.done()) {
		app.handleEvents();
		
		app.update();
		
		app.render();
	}
	
	
	app.release();
	SDL_Quit();
	
	return 0;
}
