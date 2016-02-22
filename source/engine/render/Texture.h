#ifndef TEXTURE2D_H_
#define TEXTURE2D_H_

#include "base_include.h"

#include "GraphicsPlatform.h"

// Forward declarations
class Shader;

//--------------------------------------------------
//
// Texture enums
//
//--------------------------------------------------

enum TextureColor_t {
	kTextureColorNull,
	kTextureColorRGB,
	kTextureColorRGBA
};

//--------------------------------------------------
//
// Texture2D
//
// 2D texture used in rendering
//
//--------------------------------------------------
class Texture2D {
	friend class Shader;

public:
	Texture2D();
	~Texture2D();

	// Creates the 2D texture from a buffer
	void CreateFromBuffer(TextureColor_t colorType, int width, int height, const void* data);

private:
	GLuint m_TextureId;

	TextureColor_t m_ColorType;

	int m_Width;
	int m_Height;
};

#endif