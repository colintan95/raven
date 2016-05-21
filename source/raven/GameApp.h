#ifndef GAMEAPP_H_
#define GAMEAPP_H_

#include "GameEngine.h"

//--------------------------------------------------
//
// GameApp
//
// Application class of game; contains the main loop
//
//--------------------------------------------------
class GameApp {

public:
	GameApp();
	~GameApp();

	void Run();

private:
	GameEngine* m_Engine;
};

#endif