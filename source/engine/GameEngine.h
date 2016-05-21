#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include "base_include.h"

#include "platform/platform_include.h"

#include "render/TextureRegistry.h"
#include "resource/ResourceManager.h"
#include "input/InputManager.h"
#include "render/Renderer.h"
#include "physics/PhysWorld.h"
#include "entity/EntityManager.h"
#include "entity/Scene.h"
#include "state/GameStateMachine.h"

// Forward declarations

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

	TextureRegistry* GetTextureRegistry() { return m_TexRegistry; }
	ResourceManager* GetResourceManager() { return m_Resource; }
	InputManager* GetInputManager() { return m_Input; }
	PhysWorld* GetPhysWorld() { return m_PhysWorld; }
	EntityManager* GetEntityManager() { return m_EntityManager; }
	Scene* GetScene() { return m_Scene; }
	GameStateMachine* GetStateMachine() { return m_StateMachine; }

private:
	PlatformApp* m_PlatformApp;
	PlatformInput* m_PlatformInput;	
	PlatformWindow* m_PlatformWindow;
	PlatformFileSystem* m_PlatformFileSys;

	// Engine subsystems
	TextureRegistry* m_TexRegistry;
	ResourceManager* m_Resource;
	InputManager* m_Input;
	Renderer* m_Render;

	PhysWorld* m_PhysWorld;
	EntityManager* m_EntityManager;
	Scene* m_Scene;

	GameStateMachine* m_StateMachine;

	// Shutdown engine if true
	bool m_Quit;
};

#endif