#ifndef PHYSWORLD_H_
#define PHYSWORLD_H_

#include "base_include.h"

#include "allocator/PoolAllocator.h"

#include "PhysBody.h"
#include "CollisionDetector.h"


const int kPhysWorldBodyMax = 1024;
const int kPhysWorldLayerMax = 16;

// Forward declarations
class Entity;

//--------------------------------------------------
//
// PhysWorld
//
// Stores and manages all PhysBody objects
//
//--------------------------------------------------
class PhysWorld {

public:
	PhysWorld();
	~PhysWorld();

	void Update();

	// Causes the bodies on the 2 layers to ignore each other
	void AddLayerIgnore(uint16_t layer1, uint16_t layer2);

	// Resets all the layers ignore list
	void ResetAllLayerIgnore();

	PhysBody* CreateBody(PhysBodyType_t type, uint16_t layer, Entity* entity);
	void DestroyBody(PhysBody* body); 

private:
	void CheckCollision(PhysBody* body1, PhysBody* body2);

private:
	CollisionDetector m_Detector;
	PoolAllocator<PhysBody> m_BodyPool;

	uint16_t m_LayerIgnoreMask[kPhysWorldLayerMax];

	// List containing all bodies
	PhysBody* m_ListHead;
	PhysBody* m_ListTail;
};

#endif