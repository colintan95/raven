#include "GameApp.h"

#include "GameLoadState.h"

GameApp::GameApp() {

}

GameApp::~GameApp() {

}

void GameApp::Run() {
	m_Engine = new GameEngine();

	m_Engine->Init();

	//SharedPtr<MainState> mainState(new MainState(m_Engine));
	//m_Engine->GetStateMachine()->PushState(mainState);

	SharedPtr<GameLoadState> loadState(new GameLoadState(m_Engine));
	m_Engine->GetStateMachine()->PushState(loadState);

	m_Engine->MainLoop();

	m_Engine->Shutdown();
}