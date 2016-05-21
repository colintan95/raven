#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include "base_include.h"

#include "input/InputController.h"

// Forward declarations
class MainState;

//--------------------------------------------------
//
// GameController
//
// Handles on game input
//
//--------------------------------------------------
class GameController: public IInputController {

public:
	GameController(MainState* state);
	virtual ~GameController();

	virtual bool ReceiveInput(const InputEvent& evt);

private:
	MainState* m_StatePtr;

};

#endif