#include "InputManager.h"

#include <cstring>

#include "GameEngine.h"
#include "platform/PlatformInput.h"
#include "InputController.h"

InputManager::InputManager(GameEngine* engine, PlatformInput* platformInput):
	m_InputQueue(kInputEventQueueMax) {
	ASSERT(engine != nullptr);
	ASSERT(platformInput != nullptr);

	m_EnginePtr = engine;

	m_PlatformInputPtr = platformInput;

	// Set controllers to nullptr. User must call set controller function to 
	// assign the pointers
	m_GuiController = nullptr;
	m_GameController = nullptr;

	m_InputQueue.Clear();
}

InputManager::~InputManager() {
	m_InputQueue.Clear();

	m_GameController = nullptr;
	m_GuiController = nullptr;

	m_PlatformInputPtr = nullptr;

	m_EnginePtr = nullptr;
}

void InputManager::Update() {

	InputEvent evt;

	// Clears the local input event
	memset((void*)&evt, 0, sizeof(InputEvent)); 


	// Retrieves all events from the platform input
	while (m_PlatformInputPtr->RetrieveInput(&evt)) {

		// If the event is kInputQuit, notifies the game engine to quit 
		if (evt.type == kInputQuit) {
			m_EnginePtr->QuitGame();
		}
		else if (evt.type == kInputMouseDown) {
			LOG_PRINT("%i %i", evt.mouse.x, evt.mouse.y);
		}

		// Terminates the loop if the input queue is full
		if (m_InputQueue.IsFull()) {
			break;
		}

		m_InputQueue.PushBack(evt);

		// Clears the local input event 
		memset((void*)&evt, 0, sizeof(InputEvent)); 
	}


	// Sends all events in the queue to the controllers
	while (!m_InputQueue.IsEmpty()) {
		evt = m_InputQueue.GetFront();
		m_InputQueue.PopFront();

		// Set to true if a controller handles the input
		bool handled = false;

		if (m_GuiController != nullptr) {
			handled = m_GuiController->ReceiveInput(evt);
		}

		// Only send event to game controller if gui controller did not handle
		// the event
		if (handled == false && m_GameController != nullptr) {
			handled = m_GameController->ReceiveInput(evt);
		}
	}
}

void InputManager::SetGuiController(IInputController* guiController) {
	m_GuiController = guiController;
}

void InputManager::SetGameController(IInputController* gameController) {
	m_GameController = gameController;
}