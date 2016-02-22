#ifndef RENDERER_H_
#define RENDERER_H_

#include "base_include.h"

#include "GraphicsPlatform.h"

// Forward declarations
class PlatformWindow;

//--------------------------------------------------
//
// Renderer
//
// Renders all graphical elements in the game
//
//--------------------------------------------------
class Renderer {

public:
	Renderer(PlatformWindow* platformWindow);
	~Renderer();

	// Sets up renderer for render calls
	//
	// Call before rendering anything
	void PreRender();

	// Call after rendering
	void PostRender();

	// Render all graphical elements
	void Render();

	int GetWindowWidth() const;
	int GetWindowHeight() const;

private:
	// Pointer to platform window
	PlatformWindow* m_PlatformWindowPtr;
	
};

#endif