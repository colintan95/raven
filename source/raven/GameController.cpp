#include "GameController.h"

#include "MainState.h"

#include <cstring>

GameController::GameController(MainState* state) {
	m_StatePtr = state;
}

GameController::~GameController() {
	m_StatePtr = nullptr;
}

bool GameController::ReceiveInput(const InputEvent& evt) {

	if (evt.type == kInputKeyDown) {
		switch(evt.key.code) {
		case kKeyUp:
			m_StatePtr->SetPlayerDirection(kPlayerDirUp, true);
			break;
		case kKeyDown:
			m_StatePtr->SetPlayerDirection(kPlayerDirDown, true);
			break;
		case kKeyLeft:
			m_StatePtr->SetPlayerDirection(kPlayerDirLeft, true);
			break;
		case kKeyRight:
			m_StatePtr->SetPlayerDirection(kPlayerDirRight, true);
			break;
		case kKeySpace:
			m_StatePtr->FireProjectile();
			break;
		}
	}

	if (evt.type == kInputKeyUp) {
		switch(evt.key.code) {
		case kKeyUp:
			m_StatePtr->SetPlayerDirection(kPlayerDirUp, false);
			break;
		case kKeyDown:
			m_StatePtr->SetPlayerDirection(kPlayerDirDown, false);
			break;
		case kKeyLeft:
			m_StatePtr->SetPlayerDirection(kPlayerDirLeft, false);
			break;
		case kKeyRight:
			m_StatePtr->SetPlayerDirection(kPlayerDirRight, false);
			break;
		case kKeySpace:
			m_StatePtr->ResetProjectile();
			break;
		}
	}

	return true;
}