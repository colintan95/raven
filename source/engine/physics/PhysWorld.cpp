#include "PhysWorld.h"

#include "entity/Entity.h"

#include <cstring>

PhysWorld::PhysWorld(): m_BodyPool(kPhysWorldBodyMax) {
	m_ListHead = nullptr;
	m_ListTail = nullptr;

	memset((void*)m_LayerIgnoreMask, 0, sizeof(uint16_t) * kPhysWorldLayerMax);
}

PhysWorld::~PhysWorld() {
	m_BodyPool.Clear();
}

void PhysWorld::Update() {
	PhysBody* it = m_ListHead;

	while (it != nullptr) {
		it->Update();

		// Collision checking
		PhysBody* it2 = it->m_Next;
		while (it2 != nullptr) {
			CheckCollision(it, it2);
			it2 = it2->m_Next;
		}

		it = it->m_Next;
	}
}

void PhysWorld::CheckCollision(PhysBody* body1, PhysBody* body2) {

	// Stops collision detection if both bodies are on layers that ignore each
	// other
	if ((m_LayerIgnoreMask[body1->m_Layer] & (1 << body2->m_Layer)) != 0) {
		return;
	}

	Mat3 transform1 = body1->GetTransform();

	Vec2 origin1 = body1->m_Origin;
	double width1 = body1->m_Width;
	double height1 = body1->m_Height;

	// Calculate quad coordinates for body1
	Vec3 topLeft1 = transform1 * Vec3(		-origin1.GetX(),
											-origin1.GetY(), 1.0);
	Vec3 topRight1 = transform1 * Vec3( 	-origin1.GetX() + width1,
											-origin1.GetY(), 1.0);
	Vec3 botRight1 = transform1 * Vec3( 	-origin1.GetX() + width1,
											-origin1.GetY() + height1, 1.0);
	Vec3 botLeft1 = transform1 * Vec3( 		-origin1.GetX(),
											-origin1.GetY() + height1, 1.0);

	Mat3 transform2 = body2->GetTransform();

	Vec2 origin2 = body2->m_Origin;
	double width2 = body2->m_Width;
	double height2 = body2->m_Height;

	// Calculate quad coordinates for body2
	Vec3 topLeft2 = transform2 * Vec3(		-origin2.GetX(),
											-origin2.GetY(), 1.0);
	Vec3 topRight2 = transform2 * Vec3( 	-origin2.GetX() + width1,
											-origin2.GetY(), 1.0);
	Vec3 botRight2 = transform2 * Vec3( 	-origin2.GetX() + width1,
											-origin2.GetY() + height1, 1.0);
	Vec3 botLeft2 = transform2 * Vec3( 		-origin2.GetX(),
											-origin2.GetY() + height1, 1.0);


	// Use quad collision detection if either bodies are rotated
	bool useQuadCol = false;

	if (transform1.GetRow1().GetY() != 0.0 ||
		transform1.GetRow2().GetX() != 0.0 ||
		transform2.GetRow1().GetY() != 0.0 ||
		transform2.GetRow2().GetX() != 0.0) {

		useQuadCol = true;
	}

	// Minimum translation vector
	Vec2 mtv;

	// Collision detection
	if (!useQuadCol) {

		// Use AABB collision

		Rect rect1(topLeft1.GetX(), topLeft1.GetY(), width1, height1);
		Rect rect2(topLeft2.GetX(), topLeft2.GetY(), width2, height2);

		mtv = m_Detector.AABBToAABB(rect1, rect2);
	}
	else {

		// Use Quad Collision

		Vec2 quad1[4];
		quad1[0] = Vec2(topLeft1.GetX(), topLeft1.GetY());
		quad1[1] = Vec2(topRight1.GetX(), topRight1.GetY());
		quad1[2] = Vec2(botRight1.GetX(), botRight1.GetY());
		quad1[3] = Vec2(botLeft1.GetX(), botLeft1.GetY());

		Vec2 quad2[4];
		quad2[0] = Vec2(topLeft2.GetX(), topLeft2.GetY());
		quad2[1] = Vec2(topRight2.GetX(), topRight2.GetY());
		quad2[2] = Vec2(botRight2.GetX(), botRight2.GetY());
		quad2[3] = Vec2(botLeft2.GetX(), botLeft2.GetY());

		mtv = m_Detector.QuadToQuad(quad1, quad2);
	}

	// Returns if there is no collision
	if (mtv == Vec2(0.0, 0.0)) {
		return;
	}

	// True if a collision has occurred
	bool collision = false;


	// Collision resolution for static-dynamic and static-controlled

	if (body1->m_Type == kPhysBodyStatic) {
		if (body2->m_Type == kPhysBodyDynamic || 
			body2->m_Type == kPhysBodyControlled) {

			body2->TranslateBy(-mtv);

			collision = true;
		}
	}
	else if (body2->m_Type == kPhysBodyStatic) {
		if (body1->m_Type == kPhysBodyDynamic || 
			body1->m_Type == kPhysBodyControlled) {

			body1->TranslateBy(mtv);

			collision = true;
		}
	}

	if (collision) {
		body1->m_EntityPtr->OnCollision(body2->m_EntityPtr);
		body2->m_EntityPtr->OnCollision(body1->m_EntityPtr);
	}
}

void PhysWorld::AddLayerIgnore(uint16_t layer1, uint16_t layer2) {
	ASSERT(layer1 < kPhysWorldLayerMax);
	ASSERT(layer2 < kPhysWorldLayerMax);

	m_LayerIgnoreMask[layer1] |= (1 << layer2);
	m_LayerIgnoreMask[layer2] |= (1 << layer1);
}

void PhysWorld::ResetAllLayerIgnore() {
	memset((void*)m_LayerIgnoreMask, 0, sizeof(uint16_t) * kPhysWorldLayerMax);
}

PhysBody* PhysWorld::CreateBody(PhysBodyType_t type, uint16_t layer, Entity* entity) {
	PhysBody* mem = m_BodyPool.Alloc();

	PhysBody* body = new(mem) PhysBody(type, layer, entity);

	// Adds the body to the list

	if (m_ListHead == nullptr) {
		body->m_Next = nullptr;
		body->m_Prev = nullptr;

		m_ListHead = body;
		m_ListTail = body;
	}
	else {
		body->m_Next = nullptr;
		body->m_Prev = m_ListTail;
		m_ListTail->m_Next = body;

		m_ListTail = body;
	}

	return body;
}

void PhysWorld::DestroyBody(PhysBody* body) {
	// Removes the body from the list

	if (m_ListHead == body) {
		m_ListHead = body->m_Next;
	}
	if (m_ListTail == body) {
		m_ListTail = body->m_Prev;
	}
	if (body->m_Prev != nullptr) {
		body->m_Prev->m_Next = body->m_Next;
	}
	if (body->m_Next != nullptr) {
		body->m_Next->m_Prev = body->m_Prev;
	}

	body->m_Next = nullptr;
	body->m_Prev = nullptr;

	body->~PhysBody();

	m_BodyPool.Dealloc(body);
}