#ifndef SPRITECLIP_H_
#define SPRITECLIP_H_

#include "base_include.h"

#include "math/Rect.h"

//--------------------------------------------------
//
// SpriteClip
//
// Animation clip of a sprite
//
//--------------------------------------------------
class SpriteClip {

public:
	// Parameters:
	// ... startX and startY: the position of the first frame
	// ... width and height: dimensions of each frame
	// ... columnCount and rowCount: numbers of frames per column and row
	// ... frameCount: total number of frames
	//
	// Starting from the top row, frames are ordered from left to right, top 
	// to bottom
	// E.g. columnCount = 4, rowCount = 2, frameCount = 7
	//  +---+---+---+---+
	//  | 0 | 1 | 2 | 3 |
	//  +---+---+---+---+
	//  | 4 | 5 | 6 |   |
	//  +---+---+---+---+
	SpriteClip(int startX, int startY, int width, int height, 
				int columnCount, int rowCount, int frameCount);
	~SpriteClip();

	void Update();

	void Play(bool repeat);
	void Stop();

	void Pause();
	void Resume();

	Rect GetFrame() const;

	bool IsPlaying() const { return m_Playing; }
	bool IsPaused() const { return m_Paused; }

private:
	Rect CalculateFrame() const;

private:
	// Position of the first frame
	int m_StartX;
	int m_StartY;

	// Dimensions of the frame
	int m_Width;
	int m_Height;

	// Number of frames per column and row
	int m_ColumnCount;
	int m_RowCount;

	int m_FrameCount;

	int m_CurrentFrame;

	bool m_Playing;
	bool m_Paused;
	bool m_Repeat;
};

#endif