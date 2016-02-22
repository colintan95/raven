#ifndef PLATFORMWINDOW_SDL_H_
#define PLATFORMWINDOW_SDL_H_

#include "base_include.h"

#include <SDL2/SDL.h>

// MUST be using OpenGL
#if !defined(_PLATFORM_OPENGL)
	ASSERT(0);
#endif

// Constants

const int kWindowDefaultFps = 60;

const int kWindowDefaultWidth = 640;
const int kWindowDefaultHeight = 480;

const char * const kWindowTitle = "Raven";


#if defined(_PLATFORM_OPENGL)

const int kOpenGLVersionMajor = 2;
const int kOpenGLVersionMinor = 1;

#endif

//--------------------------------------------------
//
// PlatformWindow
//
// SDL implementation for PlatformWindow
//
//--------------------------------------------------
class PlatformWindow {

public:
	PlatformWindow();

	~PlatformWindow();

	void DrawWindow();

	void ResizeWindow(int width, int height);

	void SetFramesPerSecond(int fps);

	void SetFullscreen(bool flag);

	int GetWindowWidth() const { return m_WindowWidth; }
	int GetWindowHeight() const { return m_WindowHeight; }

	bool GetFullscreen() const { return m_Fullscreen; }

private:
	SDL_Window *m_Window;
	SDL_Surface *m_ScreenSurface;
	SDL_GLContext m_GLContext;

	int m_WindowWidth;
	int m_WindowHeight;

	bool m_Fullscreen;
};

#endif