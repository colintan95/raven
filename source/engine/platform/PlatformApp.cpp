#include "PlatformApp.h"

PlatformApp::PlatformApp() {
    
    int initResult = SDL_Init(SDL_INIT_EVERYTHING);
    
	if (initResult < 0) {
		
	}
}

PlatformApp::~PlatformApp() {
	SDL_Quit();
}