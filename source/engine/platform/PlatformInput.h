#ifndef PLATFORMINPUT_H_
#define PLATFORMINPUT_H_

#include "base_include.h"

#include "input/InputEvent.h"

#include <SDL2/SDl.h>

const int kKeyStateMax = 255;
const int kKeyCodeSDLMax = 128;

// Forward declarations
class PlatformWindow;

//--------------------------------------------------
//
// PlatformInput
//
// SDL implementation for PlatformInput
//
//--------------------------------------------------
class PlatformInput {

public:
	PlatformInput(PlatformWindow* window);
	~PlatformInput();

	// Retrieves the next input in the system and places the data in evt
	//
	// Returns true if there is input to handle
	bool RetrieveInput(InputEvent* evt);

	bool GetKeyState(KeyCode_t code) const { return m_KeyStates[code]; }

	// Returns true if there is input to handle
	bool HasInput();

private:
	// Initializes the SDL key to engine key conversion table
	void InitConvertTable();

private:
	PlatformWindow* m_WindowPtr;

	// Tracks the state of all keys
	bool m_KeyStates[kKeyStateMax];
	KeyCode_t m_ConvertTable[kKeyCodeSDLMax]; // Converts SDL_KeyCode (until 128) to KeyCode_t
};

#endif