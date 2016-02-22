#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include "base_include.h"

#include "platform/platform_include.h"

// Forward declarations
class InputManager;
class Renderer;
class ResourceManager;

//--------------------------------------------------
//
// GameEngine
//
// Central engine class that contains all engine game logic and manages all
// engine systems
//
//--------------------------------------------------
class GameEngine {
	
public:
	GameEngine();
	~GameEngine();

	void Init();
	void Shutdown();

	// Main loop of the game
	void MainLoop();

	// Quits the game
	void QuitGame();

private:
	PlatformApp* m_PlatformApp;
	PlatformInput* m_PlatformInput;	
	PlatformWindow* m_PlatformWindow;
	PlatformFileSystem* m_PlatformFileSys;

	// Engine subsystems
	ResourceManager* m_Resource;
	InputManager* m_Input;
	Renderer* m_Render;

	// Shutdown engine if true
	bool m_Quit;
};

#endif