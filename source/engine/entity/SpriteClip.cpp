#include "SpriteClip.h"


SpriteClip::SpriteClip(int startX, int startY, int width, int height, 
	int columnCount, int rowCount, int frameCount) {
	ASSERT(frameCount > 0);

	m_StartX = startX;
	m_StartY = startY;

	m_Width = width;
	m_Height = height;
	
	m_ColumnCount = columnCount;
	m_RowCount = rowCount;

	m_FrameCount = frameCount;

	m_CurrentFrame = 0;
	m_Playing = false;
	m_Paused = false;
}

SpriteClip::~SpriteClip() {
	
}

void SpriteClip::Update() {
	if (m_Playing && !m_Paused) {
		if (m_CurrentFrame < (m_FrameCount - 1)) {
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

void SpriteClip::Play(bool repeat) {
	m_Playing = true;
	m_Paused = false;
	m_Repeat = repeat;
	m_CurrentFrame = 0;
}

void SpriteClip::Stop() {
	m_Playing = false;
	m_Paused = false;
	m_Repeat = false;
	m_CurrentFrame = 0;
}

void SpriteClip::Pause() {
	m_Paused = true;
}

void SpriteClip::Resume() {
	m_Paused = false;
}

Rect SpriteClip::GetFrame() const {
	return CalculateFrame();
}

Rect SpriteClip::CalculateFrame() const {
	int rowIndex = m_CurrentFrame / m_ColumnCount;
	int colIndex = m_CurrentFrame % m_ColumnCount;

	return Rect(m_StartX + colIndex * m_Width,
				m_StartY + rowIndex * m_Height,
				m_Width, m_Height);
}