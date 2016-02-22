#include "PlatformInput.h"

#include <cstring>

PlatformInput::PlatformInput() {
    InitConvertTable();
}
	
PlatformInput::~PlatformInput() {

}

bool PlatformInput::RetrieveInput(InputEvent* evt) {
	SDL_Event sdlEvent;

	memset((void*)evt, 0, sizeof(InputEvent));

	if (SDL_PollEvent(&sdlEvent) != 0) {

		// Translates each SDL event into the corresponding engine event
		if (sdlEvent.type == SDL_QUIT) {
			evt->type = kInputQuit;
		}
		else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN || sdlEvent.type == SDL_MOUSEBUTTONUP) {
			int mouseX = sdlEvent.motion.x;
			int mouseY = sdlEvent.motion.y;

			MouseCode_t code;

			switch (sdlEvent.button.button) {
				case SDL_BUTTON_LEFT:
					code = kMouseLeftButton;
					break;
				case SDL_BUTTON_RIGHT:
					code = kMouseRightButton;
					break;
				case SDL_BUTTON_MIDDLE:
					code = kMouseMiddleButton;
					break;
				default:
					code = kMouseUndefined;
			}

			evt->mouse.code = code;
			evt->mouse.x = mouseX;
			evt->mouse.y = mouseY;

			if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
				evt->type = kInputMouseDown;
			}
			else if (sdlEvent.type == SDL_MOUSEBUTTONUP) {
				evt->type = kInputMouseUp;
			}
		}
		else if (sdlEvent.type == SDL_MOUSEMOTION) {
			int mouseX = sdlEvent.motion.x;
			int mouseY = sdlEvent.motion.y;
			evt->type = kInputMouseMove;

			evt->mouse.x = mouseX;
			evt->mouse.y = mouseY;
		}
		else if (sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP) {
			SDL_Keycode sdlCode = sdlEvent.key.keysym.sym;
			KeyCode_t res = kKeyNone;

			if (sdlCode < kKeyCodeSDLMax) {
				res = m_ConvertTable[sdlCode];
			}
			else {
				switch (sdlCode) {
					case SDLK_CAPSLOCK: res = kKeyCaps; break;
					case SDLK_LSHIFT: 	res = kKeyLShift; break;
					case SDLK_RSHIFT: 	res = kKeyRShift; break;
					case SDLK_LCTRL: 	res = kKeyLCtrl; break;
					case SDLK_RCTRL: 	res = kKeyRCtrl; break;
					case SDLK_LALT: 	res = kKeyLAlt; break;
					case SDLK_RALT: 	res = kKeyRAlt; break;
					case SDLK_F1: 		res = kKeyF1; break;
					case SDLK_F2: 		res = kKeyF2; break;
					case SDLK_F3:		res = kKeyF3; break;
					case SDLK_F4: 		res = kKeyF4; break;
					case SDLK_F5: 		res = kKeyF5; break;
					case SDLK_F6: 		res = kKeyF6; break;
					case SDLK_F7: 		res = kKeyF7; break;
					case SDLK_F8: 		res = kKeyF8; break;
					case SDLK_F9: 		res = kKeyF9; break;
					case SDLK_F10: 		res = kKeyF9; break;
					case SDLK_F11: 		res = kKeyF9; break;
					case SDLK_F12: 		res = kKeyF9; break;
					case SDLK_UP: 		res = kKeyUp; break;
					case SDLK_DOWN: 	res = kKeyDown; break;
					case SDLK_LEFT: 	res = kKeyLeft; break;
					case SDLK_RIGHT: 	res = kKeyRight; break;
					default:			res = kKeyUndefined;
				}
			}

			evt->key.code = res;

			if (sdlEvent.type == SDL_KEYDOWN) {
				if (res != kKeyNone && res != kKeyUndefined) m_KeyStates[res] = true;

				evt->type = kInputKeyDown;
			}
			else if (sdlEvent.type == SDL_KEYUP) {
				if (res != kKeyNone && res != kKeyUndefined) m_KeyStates[res] = false;

				evt->type = kInputKeyUp;
			}
        }
	}
	else {
		// No input to handle
		evt->type = kInputNull;
		return false;
	}

	return true;
}

bool PlatformInput::HasInput() {
	return (SDL_PollEvent(NULL) != 0);
}

void PlatformInput::InitConvertTable() {
    memset((void*)m_ConvertTable, 0, sizeof(KeyCode_t) * kKeyCodeSDLMax);
    
	m_ConvertTable[SDLK_UNKNOWN] = kKeyNone;
	m_ConvertTable[SDLK_ESCAPE] = kKeyEscape;
	m_ConvertTable[SDLK_BACKQUOTE] = kKeyGrave;
	m_ConvertTable[SDLK_TAB] = kKeyTab;
	
	/*m_ConvertTable[SDLK_EXCLAIM] = kKeyNone;
	m_ConvertTable[SDLK_AT] = kKeyNone;
	m_ConvertTable[SDLK_QUOTEDBL] = kKeyNone;
	m_ConvertTable[SDLK_HASH] = kKeyNone;
	m_ConvertTable[SDLK_DOLLAR] = kKeyNone;
	m_ConvertTable[SDLK_PERCENT] = kKeyNone;
	m_ConvertTable[SDLK_AMPERSAND] = kKeyNone;
	m_ConvertTable[SDLK_LEFTPAREN] = kKeyNone;
	m_ConvertTable[SDLK_RIGHTPAREN] = kKeyNone;
	m_ConvertTable[SDLK_ASTERISK] = kKeyNone;*/

	m_ConvertTable[SDLK_MINUS] = kKeySubtract;
	m_ConvertTable[SDLK_EQUALS] = kKeyEquals;
	m_ConvertTable[SDLK_COMMA] = kKeyComma;
	m_ConvertTable[SDLK_PERIOD] = kKeyPeriod;
	m_ConvertTable[SDLK_SLASH] = kKeySlash;
	m_ConvertTable[SDLK_SEMICOLON] = kKeySemicolon;
	m_ConvertTable[SDLK_QUOTE] = kKeyApostrophe;
	m_ConvertTable[SDLK_BACKSLASH] = kKeyBackslash;
	m_ConvertTable[SDLK_RETURN] = kKeyReturn;
	m_ConvertTable[SDLK_SPACE] = kKeySpace;
	m_ConvertTable[SDLK_BACKSPACE] = kKeyBackspace;
	m_ConvertTable[SDLK_LEFTBRACKET] = kKeyLBrack;
	m_ConvertTable[SDLK_RIGHTBRACKET] = kKeyRBrack;

	//m_ConvertTable[SDLK_PLUS] = kKeyNone;

	m_ConvertTable[SDLK_0] = kKey0;
	m_ConvertTable[SDLK_1] = kKey1;
	m_ConvertTable[SDLK_2] = kKey2;
	m_ConvertTable[SDLK_3] = kKey3;
	m_ConvertTable[SDLK_4] = kKey4;
	m_ConvertTable[SDLK_5] = kKey5;
	m_ConvertTable[SDLK_6] = kKey6;
	m_ConvertTable[SDLK_7] = kKey7;
	m_ConvertTable[SDLK_8] = kKey8;
	m_ConvertTable[SDLK_9] = kKey9;

	/*m_ConvertTable[SDLK_COLON] = kKeyNone;
	m_ConvertTable[SDLK_LESS] = kKeyNone;
	m_ConvertTable[SDLK_GREATER] = kKeyNone;
	m_ConvertTable[SDLK_QUESTION] = kKeyNone;	
	m_ConvertTable[SDLK_CARET] = kKeyNone;
	m_ConvertTable[SDLK_UNDERSCORE] = kKeyNone;*/
	
	m_ConvertTable[SDLK_a] = kKeyA;
	m_ConvertTable[SDLK_b] = kKeyB;
	m_ConvertTable[SDLK_c] = kKeyC;
	m_ConvertTable[SDLK_d] = kKeyD;
	m_ConvertTable[SDLK_e] = kKeyE;
	m_ConvertTable[SDLK_f] = kKeyF;
	m_ConvertTable[SDLK_g] = kKeyG;
	m_ConvertTable[SDLK_h] = kKeyH;
	m_ConvertTable[SDLK_i] = kKeyI;
	m_ConvertTable[SDLK_j] = kKeyJ;
	m_ConvertTable[SDLK_k] = kKeyK;
	m_ConvertTable[SDLK_l] = kKeyL;
	m_ConvertTable[SDLK_m] = kKeyM;
	m_ConvertTable[SDLK_n] = kKeyN;
	m_ConvertTable[SDLK_o] = kKeyO;
	m_ConvertTable[SDLK_p] = kKeyP;
	m_ConvertTable[SDLK_q] = kKeyQ;
	m_ConvertTable[SDLK_r] = kKeyR;
	m_ConvertTable[SDLK_s] = kKeyS;
	m_ConvertTable[SDLK_t] = kKeyT;
	m_ConvertTable[SDLK_u] = kKeyU;
	m_ConvertTable[SDLK_v] = kKeyV;
	m_ConvertTable[SDLK_w] = kKeyW;
	m_ConvertTable[SDLK_x] = kKeyX;
	m_ConvertTable[SDLK_y] = kKeyY;
	m_ConvertTable[SDLK_z] = kKeyZ;

	//m_ConvertTable[SDLK_DELETE] = kKeyNone;
}