#ifndef IMAGE_H_
#define IMAGE_H_

#include "base_include.h"

#include "Texture.h"

//--------------------------------------------------
//
// Image
//
// Contains an image that is rendered by the entity and gui system
//
//--------------------------------------------------
class Image {

public:
	Image();
	~Image();

	Texture2D GetTexture() const { return m_Texture; }

private:
	// Texture to pass to renderer
	Texture2D m_Texture;

	int m_Width;
	int m_Height;
};

#endif