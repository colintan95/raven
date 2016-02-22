#ifndef COLLISIONDETECTOR_H_
#define COLLISIONDETECTOR_H_

#include "base_include.h"

#include "math/Vector.h"
#include "math/Rect.h"

//--------------------------------------------------
//
// Projection of a shape onto a 1-D axis
//
//--------------------------------------------------
struct CollisionProjection {
	double min;
	double max;
};

//--------------------------------------------------
//
// CollisionDetector
//
// Utility class that tests for collisions using multiple algorithms
//
//--------------------------------------------------
class CollisionDetector {

public:
	// Test if the point is within the AABB
	bool PointToAABB(const Vec2& pt, const Rect& rect);

	// Tests if the line intersects the AABB
	//
	// If true, returns the point of contact. Else, returns the zero vector
	Vec2 LineToAABB(const Vec2& linePt, const Vec2& lineDir, const Rect& rect);

	// Checks collision between two slow or non moving objects
	//
	// Returns the min translation vector w.r.t to rect1 (how much rect1 
	// should move)
	// Parameter rect1 must be the AABB after the object moved
	Vec2 AABBToAABB(const Rect& rect1, const Rect& rect2);

	// Checks collision between two quads of specified size
	//
	// Returns the min translation vector
	// Quads need not be axis-aligned
	//
	// Uses the separating axis thereoem
	Vec2 QuadToQuad(const Vec2 quad1[4], const Vec2 quad2[4]);

private:
	// Parameter axis must be normalized
	CollisionProjection ProjectQuadToAxis(const Vec2 quad[4], const Vec2& axis);

	double TestProjectionOverlap(const CollisionProjection& proj1, const CollisionProjection& proj2);
};

#endif