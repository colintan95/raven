#ifndef GAMELOADSTATE_H_
#define GAMELOADSTATE_H_

#include "base_include.h"

#include "state/BaseGameState.h"

// Forward declarations
class GameEngine;
class ResourceManager;

//--------------------------------------------------
//
// GameLoadState
//
// Loads all the data needed for the game
//
// Called between the main menu and the main state
//
//--------------------------------------------------
class GameLoadState: public BaseGameState {

public:
	GameLoadState(GameEngine* engine);
	virtual ~GameLoadState();

	virtual void Update();

	virtual void PreState();
	virtual void PostState();
	
	virtual void Pause();
	virtual void Resume();

private:
	ResourceManager* m_ResourcePtr;

	// True if loading is completed
	bool m_LoadingDone;
};

#endif