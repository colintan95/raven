#include "GameEngine.h"


GameEngine::GameEngine() {
	m_Quit = false;
}

GameEngine::~GameEngine() {

}

void GameEngine::Init() {
	// Sets up all platform specific subsystems
	// MUST be called FIRST
	m_PlatformApp = new PlatformApp();
	m_PlatformWindow = new PlatformWindow();
	m_PlatformInput = new PlatformInput(m_PlatformWindow);

	m_PlatformFileSys = new PlatformFileSystem();

	m_TexRegistry = new TextureRegistry();
	m_Resource = new ResourceManager(m_PlatformFileSys, m_TexRegistry);
	m_Input = new InputManager(this, m_PlatformInput);
	m_Render = new Renderer(m_PlatformWindow);

	m_PhysWorld = new PhysWorld();
	m_EntityManager = new EntityManager(m_PhysWorld);
	m_Scene = new Scene(m_PlatformWindow);

	m_StateMachine = new GameStateMachine();
}

void GameEngine::Shutdown() {
	delete m_StateMachine;

	delete m_Scene;
	delete m_EntityManager;
	delete m_PhysWorld;

	delete m_Render;
	delete m_Input;
	delete m_Resource;
	delete m_TexRegistry;

	delete m_PlatformFileSys;
	delete m_PlatformInput;
	delete m_PlatformWindow;
	delete m_PlatformApp;
}

void GameEngine::MainLoop() {

	while (!m_Quit) {
		m_StateMachine->Update();

		// Update the resource manager;
		m_Resource->Update();

		// Update input
		m_Input->Update();

		// Update physics
		m_PhysWorld->Update();

		// Update scene
		m_Scene->Update();


		// Rendering logic

		// Sets up before any rendering
		m_Render->PreRender();

		//m_Render->Render();

		m_Scene->Render();

		m_Render->PostRender();
	}
}

void GameEngine::QuitGame() {
	m_Quit = true;
}