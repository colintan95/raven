#include "Texture.h"

Texture::Texture() {
	m_TextureId = 0;
	m_TextureUnit = 0;

	m_ColorType = kTextureColorNull;

	m_Width = 0;
	m_Height = 0;
}

Texture::~Texture() {
	glDeleteTextures(1, &m_TextureId);
}

void Texture::CreateFromBuffer(TextureColor_t colorType, int width, int height, const void* data) {
	ASSERT(width > 0 && height > 0);

	glGenTextures(1, &m_TextureId);

	glBindTexture(GL_TEXTURE_2D, m_TextureId);

	// (TODO:) Allow texture units to vary
	glActiveTexture(GL_TEXTURE0 + 1);
	m_TextureUnit = 1;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (colorType == kTextureColorRGBA) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else if (colorType == kTextureColorRGB) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	m_ColorType = colorType;

	m_Width = width;
	m_Height = height;
}