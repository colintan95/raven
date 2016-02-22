#include "GameEngine.h"

int main(int argc, char* argv[]) {

	GameEngine* engine = new GameEngine();

	engine->Init();

	engine->MainLoop();

	engine->Shutdown();
	
	return 0;
}