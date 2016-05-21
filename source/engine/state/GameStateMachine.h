#ifndef GAMESTATEMACHINE_H_
#define GAMESTATEMACHINE_H_

#include "base_include.h"

#include "BaseGameState.h"

#include "container/Stack.h"

const int kGameStateMax = 16; // Maximum number of game states present at one time

//--------------------------------------------------
//
// GameStateMachine
//
// Contains all game state information
//
//--------------------------------------------------
class GameStateMachine {

public:
	GameStateMachine();

	// Pops all states off the stack
	//
	// PostTrans() or Resume() will NOT be called
	~GameStateMachine();

	// Calls Update() for the top state
	void Update();

	// Changes the top state with the specified state. The previous state is 
	// removed from the stack
	//
	// There must be a state already on the stack
	void ChangeState(SharedPtr<BaseGameState> state);

	// Pushes the state on top of the stack
	//
	// Pauses the previous state
	void PushState(SharedPtr<BaseGameState> state);

	// Pops the top state off the stack
	//
	// Resumes the previous state
	void PopState();

	BaseGameState* GetTopState();

private:
	Stack<SharedPtr<BaseGameState> > m_Stack;

private:
	// Uncopyable
	GameStateMachine(const GameStateMachine&);
	GameStateMachine& operator=(const GameStateMachine&);

};

#endif