#ifndef ENTITY_H_
#define ENTITY_H_

#include "base_include.h"

#include "math/Vector.h"
#include "math/Matrix.h"


typedef uint64_t EntityId_t;


#define CONCRETE_PROTOTYPE(classname)	\
public:	\
	static Entity* CreateInstance();


#define CONCRETE_DECLARATION(classname)	\
Entity* className::CreateInstance() {	\
	classname* ptr = new classname;	\
	return ptr;	\
}


// Forward declarations
class Scene;
class Sprite;

//--------------------------------------------------
//
// Entity
//
// Main game object
//
//--------------------------------------------------
class Entity {
	friend class Scene;

public:
	Entity();
	virtual ~Entity();

	virtual void Update();
	

	void TranslateBy(const Vec2& vec);
	void RotateBy(double rotation);

	void TranslateTo(const Vec2& vec);
	void RotateTo(double rotation);


	void AddChild(Entity* entity);
	void RemoveChild(Entity* entity);


	void SetSprite(SharedPtr<Sprite> sprite) {
		m_Sprite = sprite;
	}
	

	EntityId_t GetId() const { return m_Id; }
	Sprite* GetSprite() { return m_Sprite.get(); }

	Mat3 GetWorldTransform() const { return m_WorldTransform; }

	bool HasSprite() const { return m_Sprite.get() != nullptr; }

private:
	// Call if any transform has been modified
	//
	// Also updates the transform for all children
	void UpdateAllTransform();

	// Calculates the local transform using local position and rotation
	void CalcLocalTransform();

	// Calculates the world transform by multiplying its parent's world
	// transform and its local transform. Also updates the transform of each
	// child below it in the hierarchy
	//
	// Costly operation as it will recursively call this method for all nodes
	// below this node in the hierarcy
	void CalcWorldAndChildrenTransform();

protected:
	// Unique id of the entity; assigned during construction
	EntityId_t m_Id;


	Entity* m_Parent;
	// Points to the head of the children list
	Entity* m_Children;
	// Points to the next sibling of its parent's children list
	Entity* m_Sibling; 


	// Transform obtained from the m_LocalPosition and m_Rotation
	Mat3 m_LocalTransform;

	// Cached world transform
	//
	// Will be re-calculated if the local transform changes, or the parent's
	// world transform is changed
	Mat3 m_WorldTransform;


	// Pointer to sprite if present
	SharedPtr<Sprite> m_Sprite;


	// Position and rotation relative to parent
	Vec2 m_Position;
	float m_Rotation; // Rotation in radians
};

#endif