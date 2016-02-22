#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#include "base_include.h"

#include "InputEvent.h"

#include "container/Queue.h"

// Max number of input events stored in InputManager
const int kInputEventQueueMax = 1024;

// Forward declarations
class GameEngine;
class PlatformInput;
class IInputController;

//--------------------------------------------------
//
// InputManager
//
// Processes all input from the platform
//
// Stores all input events from the system and sends the events to the GUI
// and Game controller, both of which are subclasses of IInputController
//
//--------------------------------------------------
class InputManager {

public:
	InputManager(GameEngine* engine, PlatformInput* platformInput);
	~InputManager();

	// Polls input from the platform input manager and sends the input events
	// to the controllers
	//
	// The gui controller has precedence over the game controller. The input 
	// is not sent to the game controller if the gui has already handled the 
	// input
	//
	// If the input is a quit event, method calls engine to shut down the game
	void Update();

	// Sets the pointer to a specific controller
	void SetGuiController(IInputController* guiController);
	void SetGameController(IInputController* gameController);

private:
	// Game engine pointer
	GameEngine* m_EnginePtr;

	// Platform input pointer
	PlatformInput* m_PlatformInputPtr;

	// Gui and game controller pointers
	IInputController* m_GuiController;
	IInputController* m_GameController;


	// Input events that are yet to be handled
	Queue<InputEvent> m_InputQueue;
};


#endif