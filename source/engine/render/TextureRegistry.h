#ifndef TEXTUREREGISTRY_H_
#define TEXTUREREGISTRY_H_

#include "base_include.h"

#include "Texture.h"

#include "container/HashMap.h"

//--------------------------------------------------
//
// TextureRegistry
//
// Creates and tracks all textures
//
//--------------------------------------------------
class TextureRegistry {

public:
	TextureRegistry();
	~TextureRegistry();

	Texture* CreateTexture(const char* name, TextureColor_t colorType, int width, int height, const void* data);

	Texture* GetTexture(const char* name);

private:
	HashMap<const char*, SharedPtr<Texture> > m_TexTable;
};

#endif