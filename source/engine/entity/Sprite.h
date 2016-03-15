#ifndef SPRITE_H_
#define SPRITE_H_

#include "base_include.h"

#include "math/Vector.h"
#include "math/Rect.h"
#include "container/HashMap.h"

//--------------------------------------------------
//
// SpriteClip
//
// Animation clip of a sprite
//
// Starting from the top row, frames are ordered from left to right, top 
// to bottom
// E.g. columnCount = 4, rowCount = 2, frameCount = 7
//  +---+---+---+---+
//  | 0 | 1 | 2 | 3 |
//  +---+---+---+---+
//  | 4 | 5 | 6 |   |
//  +---+---+---+---+
//
//--------------------------------------------------
struct SpriteClip {
	// Position of the first frame
	int startX;
	int startY;

	// Dimensions of each frame
	int width;
	int height;

	// Number of frames per column and row
	int columnCount;
	int rowCount;

	// Total number of frames in the animation
	int frameCount;
};


// Forward declarations
class Texture;

//--------------------------------------------------
//
// Sprite
//
// Graphical element of the entity
//
//--------------------------------------------------
class Sprite {

public:
	Sprite(Texture* texture, int clipCount);
	~Sprite();

	void Update();

	// Sprite clip methods
	void AddClip(const char* name, int startX, int startY, int width, int height, int columnCount, int rowCount, int frameCount);

	void PlayClip(const char* name, bool repeat);
	void StopClip();

	void PauseClip();
	void ResumeClip();

	void SetOrigin(Vec2 origin) { m_Origin = origin; }

	Vec2 GetOrigin() const { return m_Origin; }

	// Returns the texcoord frame in the texture
	Rect GetFrame() const;

	Texture* GetTexture() { return m_TexturePtr; }

private:
	// Pointer to the texture containing the sprite graphics
	Texture* m_TexturePtr;

	// Table containing all sprite clips
	HashMap<const char*, SpriteClip> m_ClipTable;

	// Point in the sprite that will be aligned to the entity
	//
	// Offset from the top left corner
	Vec2 m_Origin;

	// Variables for running sprite clip
	SpriteClip* m_CurrentClip;

	int m_CurrentFrame;

	bool m_Playing;
	bool m_Paused;
	bool m_Repeat;
};


#endif