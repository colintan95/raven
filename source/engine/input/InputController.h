#ifndef INPUTCONTROLLER_H_
#define INPUTCONTROLLER_H_

#include "base_include.h"

#include "InputEvent.h"

//--------------------------------------------------
//
// IInputController
//
// Interface for controller that handles input received from InputManager
//
//--------------------------------------------------
class IInputController {

public:
	virtual ~IInputController() {}

	// Receives an input event
	//
	// Returns true if the controller handled the event
	virtual bool ReceiveInput(const InputEvent& evt) = 0;

};


#endif