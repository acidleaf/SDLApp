#include "pch.h"
#include "App.h"

int main(int argc, char* argv[]) {
	
	auto app = App::getInstance();
	if (!app->init("SDLApp", 1280, 700, argc, (const char**)argv)) {
		printf("Error initializing app\n");
		return -1;
	}
	
	app->run();
	app->release();
	
	return 0;
}