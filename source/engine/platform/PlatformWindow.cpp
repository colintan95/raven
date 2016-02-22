#include "PlatformWindow.h"

PlatformWindow::PlatformWindow() {	
    m_WindowWidth = 0;
    m_WindowHeight = 0;
    m_Fullscreen = false;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, kOpenGLVersionMajor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, kOpenGLVersionMinor);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    m_Window = SDL_CreateWindow(kWindowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, kWindowDefaultWidth, kWindowDefaultHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    
    if (m_Window == NULL) {
    	LOG_FATAL_ERROR("SDL Window could not be created. SDL Error: %s\n", SDL_GetError());
    }

    m_ScreenSurface = SDL_GetWindowSurface(m_Window);

    m_GLContext = SDL_GL_CreateContext(m_Window);

    if (m_GLContext == NULL) {
    	LOG_FATAL_ERROR("SDL OpenGL Context could not be created. SDl Error: %s\n", SDL_GetError());
    }

    // Synchronizes updates with vertical retrace
    SDL_GL_SetSwapInterval(1);
}

PlatformWindow::~PlatformWindow() {
	SDL_GL_DeleteContext(m_GLContext);
	SDL_DestroyWindow(m_Window);
}

void PlatformWindow::DrawWindow() {
    SDL_GL_SwapWindow(m_Window);
}

void PlatformWindow::ResizeWindow(int width, int height) {
    ASSERT(width > 0);
    ASSERT(height > 0);

    // Fullscreen windows cannot be resized
    if (m_Fullscreen) {
        return;
    }

    SDL_SetWindowSize(m_Window, width, height);

    m_WindowWidth = width;
    m_WindowHeight = height;
}

void PlatformWindow::SetFramesPerSecond(int fps) {
    // (TODO:)
}

void PlatformWindow::SetFullscreen(bool flag) {
    if (flag) {
        SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN);

        m_Fullscreen = true;
    }
    else {
        SDL_SetWindowFullscreen(m_Window, 0);

        m_Fullscreen = false;
    }
}