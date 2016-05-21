#ifndef IGAMESTATE_H_
#define IGAMESTATE_H_

#include "base_include.h"

#include "input/InputEvent.h"

// Forward declarations
class GameEngine;
//--------------------------------------------------
//
// BaseGameState
//
// Base for all game states
//
//--------------------------------------------------
class BaseGameState {

public:
	BaseGameState(GameEngine* engine);
	virtual ~BaseGameState();

	// Call on each frame of the game loop
	virtual void Update() = 0;


	// Call when the game state changes from another state to this state
	//
	// Called after the previous state's PosTrans()
	virtual void PreState() = 0;

	// Call when the game state changes from this state to another state
	//
	// Called before the next state's PreTrans()
	virtual void PostState() = 0;


	// Pauses the state
	//
	// Called when another state is pushed on top of this state
	virtual void Pause() = 0;

	// Resume the state
	//
	// Called when the state above this state is popped off the stack
	virtual void Resume() = 0;
	
protected:
	GameEngine* m_EnginePtr;

};

#endif