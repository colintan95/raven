#include "GameLoadState.h"

#include "MainState.h"

GameLoadState::GameLoadState(GameEngine* engine): BaseGameState(engine) {
	m_LoadingDone = false;

	m_ResourcePtr = engine->GetResourceManager();
}

GameLoadState::~GameLoadState() {
	
}

void GameLoadState::Update() {

	if (m_ResourcePtr->GetResource("metalslug.png").IsValid() &&
		m_ResourcePtr->GetResource("projectile.png").IsValid() &&
		m_ResourcePtr->GetResource("ms_enemy.png").IsValid()) 
	{
		m_LoadingDone = true;
	}

	if (m_LoadingDone) {
		// Changes to the main state after loading is complete
		SharedPtr<MainState> mainState(new MainState(m_EnginePtr));
		m_EnginePtr->GetStateMachine()->ChangeState(mainState);
	}
}

void GameLoadState::PreState() {
	m_ResourcePtr->LoadResourceFromFile("metalslug.png", kResourceTypePng);
	m_ResourcePtr->LoadResourceFromFile("projectile.png", kResourceTypePng);
	m_ResourcePtr->LoadResourceFromFile("ms_enemy.png", kResourceTypePng);
}

void GameLoadState::PostState() {

}
	
void GameLoadState::Pause() {

}

void GameLoadState::Resume() {
	
}