#include "BaseGameState.h"

#include "GameEngine.h"

BaseGameState::BaseGameState(GameEngine* engine) {
	m_EnginePtr = engine;
}

BaseGameState::~BaseGameState() {
	m_EnginePtr = nullptr;
}