#ifndef GRAPHICSPLATFORM_H_
#define GRAPHICSPLATFORM_H_

#include "base_include.h"

//--------------------------------------------------
//
// GraphicsPlatform.h
//
// Includes all headers needed by the native graphics platform
//
//--------------------------------------------------

#include <SDL2/SDL.h>

#if defined(_PLATFORM_OSX)
#include <OpenGL/gl.h>
#else 
#include <GL/gl.h>
#endif


#endif