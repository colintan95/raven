#ifndef PHYSBODY_H_
#define PHYSBODY_H_

#include "base_include.h"

#include "math/Vector.h"
#include "math/Matrix.h"

// static - not affected by physics
// dynamic - affected by physics
// controlled - controlled directly by player
enum PhysBodyType_t {
	kPhysBodyNone = 0,
	kPhysBodyStatic,
	kPhysBodyDynamic,
	kPhysBodyControlled
};

// Forward declarations
class Entity;
class PhysWorld;

//--------------------------------------------------
//
// PhysBody
//
// Object in the physics engine
//
// Child entities should NOT have a PhysBody
//
//--------------------------------------------------
class PhysBody {
	friend class PhysWorld;

public:
	PhysBody();
	PhysBody(PhysBodyType_t type, uint16_t layer, Entity* entity);
	~PhysBody();

	void Update();

	// Updates the body properties using information from the entity
	void UpdateFromEntity();

	// Use ONLY these functions to modify the body location
	void TranslateBy(const Vec2& vec);
	void TranslateTo(const Vec2& vec);

	void SetVelocity(const Vec2& vec) { m_Velocity = vec; }

	void SetOrigin(const Vec2& vec) { m_Origin = vec; }
	void SetWidth(double width) { m_Width = width; }
	void SetHeight(double height) { m_Height = height; }

	uint16_t GetLayer() const { return m_Layer; }

	Mat3 GetTransform() const { return m_Transform; }

	Vec2 GetOrigin() const { return m_Origin; }
	double GetWidth() const { return m_Width; }
	double GetHeight() const { return m_Height; }

	
private:
	Entity* m_EntityPtr;

	PhysBodyType_t m_Type;

	// Index of layer that the body is in
	uint16_t m_Layer;

	// PhysBody list pointers
	PhysBody* m_Next;
	PhysBody* m_Prev;

	// Point in the sprite that will be aligned to the entity
	//
	// Offset from the top left corner
	Vec2 m_Origin;

	// Transform of the entity
	Mat3 m_Transform;

	Vec2 m_Velocity;
	//Vec2 m_Acceleration;

	double m_Width;
	double m_Height;
};

#endif