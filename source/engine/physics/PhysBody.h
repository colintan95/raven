#ifndef PHYSBODY_H_
#define PHYSBODY_H_

#include "base_include.h"

#include "math/Vector.h"
#include "math/Matrix.h"


//--------------------------------------------------
//
// Data that PhysBody will update if there is a collision
//
//--------------------------------------------------
struct CollisionUpdate {
	Vec2 position;
};

//--------------------------------------------------
//
// PhysBody
//
// Object in the physics engine
//
//--------------------------------------------------
class PhysBody {

public:
	PhysBody();
	~PhysBody();

private:
	Vec2 m_Position;
	double m_Rotation;

	double m_Width;
	double m_Height;

	bool m_Rotatable;
};

#endif