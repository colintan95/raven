#include "TextureRegistry.h"

TextureRegistry::TextureRegistry(): m_TexTable(kTextureCountMax) {

}

TextureRegistry::~TextureRegistry() {

}

Texture* TextureRegistry::CreateTexture(const char* name, TextureColor_t colorType, int width, int height, const void* data) {

	if (GetTexture(name) != nullptr) {
		LOG_ERROR("TextureRegistry: texture \'%s\' already exists", name);
		return nullptr;
	}

	SharedPtr<Texture> texture(new Texture());

	texture->CreateFromBuffer(colorType, width, height, data);

	m_TexTable.Insert(name, texture);

	return texture.get();
}

Texture* TextureRegistry::GetTexture(const char* name) {
	auto it = m_TexTable.Find(name);

	if (it == m_TexTable.End()) {
		return nullptr;
	}
	else {
		return it.GetValue().get();
	}
}