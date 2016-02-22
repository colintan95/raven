#ifndef INPUTEVENT_H_
#define INPUTEVENT_H_

//--------------------------------------------------
//
// InputEvent.h
//
// Defines the InputEvent data structure
// Defines enums for codes used in an input event
//
//--------------------------------------------------

#include "base_include.h"


//--------------------------------------------------
//
// Codes for all universal keyboard and mouse buttons
//
//--------------------------------------------------
const int kKeyCodeMax = 255;

enum KeyCode_t {
	kKeyNone = 0,
	kKeyUndefined,
	kKeyEscape,
	kKeyGrave,
	kKeyTab,
	kKeyCaps,

	kKeyLShift, kKeyRShift,
	kKeyLCtrl, kKeyRCtrl,
	kKeyLAlt, kKeyRAlt,

	kKeyF1, kKeyF2, kKeyF3, kKeyF4, kKeyF5, kKeyF6,
	kKeyF7, kKeyF8, kKeyF9, kKeyF10, kKeyF11, kKeyF12,

	kKey1, kKey2, kKey3, kKey4, kKey5,
	kKey6, kKey7, kKey8, kKey9, kKey0,

	kKeyA, kKeyB, kKeyC, kKeyD, kKeyE, kKeyF, kKeyG,
	kKeyH, kKeyI, kKeyJ, kKeyK, kKeyL, kKeyM, kKeyN,
	kKeyO, kKeyP, kKeyQ, kKeyR, kKeyS, kKeyT, kKeyU,
	kKeyV, kKeyW, kKeyX, kKeyY, kKeyZ,

	kKeyUp, kKeyDown, kKeyLeft, kKeyRight,

	kKeySubtract, kKeyEquals, kKeyComma, kKeyPeriod,
	kKeySlash, kKeySemicolon, kKeyApostrophe, kKeyBackslash,
	kKeyReturn, kKeySpace, kKeyBackspace, kKeyLBrack, kKeyRBrack
};

enum MouseCode_t {
	kMouseLeftButton,
	kMouseRightButton,
	kMouseMiddleButton,
	kMouseUndefined
};

//--------------------------------------------------
//
// Input event types
//
//--------------------------------------------------
enum InputEventType_t {
	kInputNull,
	kInputQuit,
	kInputMouseDown,
	kInputMouseUp,
	kInputMouseMove,
	kInputKeyDown,
	kInputKeyUp
};

//--------------------------------------------------
//
// Mouse and Keyboard data in InputEvent
//
//--------------------------------------------------

struct InputEventMouse {
    MouseCode_t code;
    int x;
    int y;
};

struct InputEventKey {
    KeyCode_t code;
};


//--------------------------------------------------
//
// InputEvent
//
// Contains the input information for each input event
//
//--------------------------------------------------
struct InputEvent {
	InputEventType_t type;
	union {
        InputEventMouse mouse;
        InputEventKey key;
	};
};


#endif