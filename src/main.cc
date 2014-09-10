#include "App.h"


// A base SDL App that displays an empty window

int main(int argc,  char * argv[]) {
	
	App* app = App::getInstance();
	
	if (!app->init("SDLApp", 800, 600)) {
		printf("Failed initializing app\n");
		return -1;
	}
	
	while (!app->done()) {
		app->handleEvents();
		
		app->update();
		
		app->render();
	}
	
	
	app->release();
	
	delete app;
	return 0;
}
