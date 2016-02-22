#include "GameEngine.h"

#include "resource/ResourceManager.h"
#include "input/InputManager.h"
#include "render/Renderer.h"

GameEngine::GameEngine() {
	m_Quit = false;
}

GameEngine::~GameEngine() {

}

void GameEngine::Init() {
	// Sets up all platform specific subsystems
	// MUST be called FIRST
	m_PlatformApp = new PlatformApp();
	m_PlatformInput = new PlatformInput();
	m_PlatformWindow = new PlatformWindow();
	m_PlatformFileSys = new PlatformFileSystem();

	m_Resource = new ResourceManager(m_PlatformFileSys);
	m_Input = new InputManager(this, m_PlatformInput);
	m_Render = new Renderer(m_PlatformWindow);
}

void GameEngine::Shutdown() {
	delete m_Render;
	delete m_Input;
	delete m_Resource;

	delete m_PlatformFileSys;
	delete m_PlatformWindow;
	delete m_PlatformInput;
	delete m_PlatformApp;
}

void GameEngine::MainLoop() {

	m_Resource->LoadResourceFromFile("img.jpg");

	for (int i = 0; i < 10; ++i) {
	    m_Resource->LoadResourceFromFile("img1.jpg");
	    m_Resource->LoadResourceFromFile("img2.jpg");
	    m_Resource->LoadResourceFromFile("img3.jpg");
	}

	while (!m_Quit) {
		// Update the resource manager;
		m_Resource->Update();

		// Update input
		m_Input->Update();


		// Rendering logic

		// Sets up before any rendering
		m_Render->PreRender();

		m_Render->Render();

		m_Render->PostRender();
	}
}

void GameEngine::QuitGame() {
	m_Quit = true;
}