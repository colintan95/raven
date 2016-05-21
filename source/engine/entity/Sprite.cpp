#include "Sprite.h"

#include "render/Texture.h"

Sprite::Sprite(Texture* texture, int clipCount): m_ClipTable(clipCount) {
	m_TexturePtr = texture;

	m_Origin = Vec2(0.0, 0.0);

	m_FlipX = false;
	m_FlipY = false;

	m_CurrentClip = nullptr;

	m_CurrentFrame = 0;

	m_Playing = false;
	m_Paused = false;
	m_Repeat = false;
}

Sprite::~Sprite() {
	m_TexturePtr = nullptr;

	m_CurrentClip = nullptr;
}

void Sprite::Update() {

	if (m_CurrentClip != nullptr) {
		if (m_Playing && !m_Paused) {
			if (m_CurrentFrame < (m_CurrentClip->frameCount - 1)) {
				++m_CurrentFrame;
			}
			else if (m_Repeat) {
				m_CurrentFrame = 0;
			}
			else {
				m_Playing = false;
				m_Paused = false;
			}
		}
	}
}

void Sprite::AddClip(const char* name, int startX, int startY, int width, int height, int columnCount, int rowCount, int frameCount) {

	ASSERT(!m_ClipTable.IsFull());

	SpriteClip clip;
	clip.startX = startX;
	clip.startY = startY;
	clip.width = width;
	clip.height = height;
	clip.columnCount = columnCount;
	clip.rowCount = rowCount;
	clip.frameCount = frameCount;

	m_ClipTable.Insert(name, clip);

	if (m_CurrentClip == nullptr) {
		m_CurrentClip = &m_ClipTable.Find(name).GetValue();
	}
}

void Sprite::PlayClip(const char* name, bool repeat) {
	auto it = m_ClipTable.Find(name);

	if (it == m_ClipTable.End()) {
		LOG_ERROR("Sprite: could not find clip \'%s\'", name);
		return;
	}

	m_CurrentClip = &it.GetValue();

	m_Playing = true;
	m_Paused = false;
	m_Repeat = repeat;
	m_CurrentFrame = 0;
}

void Sprite::StopClip() {
	m_Playing = false;
	m_Paused = false;
	m_Repeat = false;
	m_CurrentFrame = 0;
}

void Sprite::PauseClip() {
	m_Paused = true;
}

void Sprite::ResumeClip() {
	m_Paused = false;
}

Rect Sprite::GetFrame() const {
	if (m_CurrentClip == nullptr) {
		return Rect(0.0, 0.0, 0.0, 0.0);
	}

	int rowIndex = m_CurrentFrame / m_CurrentClip->columnCount;
	int colIndex = m_CurrentFrame % m_CurrentClip->columnCount;

	return Rect(m_CurrentClip->startX + colIndex * m_CurrentClip->width,
				m_CurrentClip->startY + rowIndex * m_CurrentClip->height,
				m_CurrentClip->width, m_CurrentClip->height);
}