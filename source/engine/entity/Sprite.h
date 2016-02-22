#ifndef SPRITE_H_
#define SPRITE_H_

#include "base_include.h"

#include "math/Vector.h"

// Forward declarations
class SpriteClip;

//--------------------------------------------------
//
// Sprite
//
// Graphical element of the entity
//
//--------------------------------------------------
class Sprite {

public:
	Sprite();
	~Sprite();

	void Update();

	// Sprite clip methods

	void PlayClip();
	void StopClip();

	void PauseClip();
	void ResumeClip();

private:

	SpriteClip* m_CurrentClip;

	// Point in the sprite that will be aligned to the entity
	//
	// Offset from the top left corner
	Vec2 m_Origin;
};


#endif