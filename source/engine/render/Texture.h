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

// Max number of textures in the game
const int kTextureCountMax = 16;

//--------------------------------------------------
//
// Texture
//
// 2D texture used in rendering
//
//--------------------------------------------------
class Texture {
	friend class Shader;

public:
	Texture();
	~Texture();

	// Creates the 2D texture from a buffer
	void CreateFromBuffer(TextureColor_t colorType, int width, int height, const void* data);

	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

private:
	GLuint m_TextureId;
	GLuint m_TextureUnit;

	TextureColor_t m_ColorType;

	int m_Width;
	int m_Height;
};

#endif