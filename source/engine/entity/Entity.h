#ifndef ENTITY_H_
#define ENTITY_H_

#include "base_include.h"

#include "math/Vector.h"
#include "math/Matrix.h"


typedef uint64_t EntityId_t;


#define ABSTRACT_PROTOTYPE(classname)	\
public:	\
	virtual TypeInfo* GetType() const;	\
private:


#define ABSTRACT_DECLARATION(classname)	\
TypeInfo* classname::GetType() const {	\
	ASSERT(0);	\
	return nullptr;	\
}


#define CONCRETE_PROTOTYPE(classname)	\
private:	\
	static TypeInfo m_Type;	\
public:	\
	static Entity* CreateInstance();	\
	static void DestroyInstance(Entity* ptr);	\
	virtual TypeInfo* GetType() const;	\
private:


#define CONCRETE_DECLARATION(classname)	\
TypeInfo classname::m_Type(#classname, (Entity* (*)())\
	&classname::CreateInstance, (void (*)(Entity*))	\
	&classname::DestroyInstance, (void (Entity::*)())&classname::Spawn);	\
Entity* classname::CreateInstance() {	\
	classname* ptr = new classname;	\
	return ptr;	\
}	\
void classname::DestroyInstance(Entity* ptr) {	\
	classname* castPtr = (classname*)ptr;	\
	delete castPtr;	\
}	\
TypeInfo* classname::GetType() const {	\
	return &m_Type; \
}


// Forward declarations
class EntityManager;
class Scene;
class Sprite;
class PhysBody;
class TypeInfo;

//--------------------------------------------------
//
// Entity
//
// Main game object
//
//--------------------------------------------------
class Entity {
	ABSTRACT_PROTOTYPE(Entity)

	friend class EntityManager;
	friend class Scene;

public:
	Entity();
	virtual ~Entity();

	// Calls Update()
	void EntityUpdate();

	virtual void Spawn() = 0;

	virtual void Update() = 0;

	// Called when a collision occurs with the specified entity
	virtual void OnCollision(Entity* entity) = 0;
	
	void TranslateBy(const Vec2& vec);
	void RotateBy(double rotation);

	void TranslateTo(const Vec2& vec);
	void RotateTo(double rotation);

	// Flips the entity's sprite
	void FlipX(bool flag);
	void FlipY(bool flag);


	void AddChild(Entity* entity);
	void RemoveChild(Entity* entity);

	// Sets the layer index for this entity and all of its children
	void SetLayerIndex(int index);

	void SetSprite(SharedPtr<Sprite> sprite) {
		m_Sprite = sprite;
	}

	Vec2 GetWorldPosition() const;

	bool GetFlipX() const;
	bool GetFlipY() const;

	EntityId_t GetId() const { return m_Id; }
	Sprite* GetSprite() { return m_Sprite.get(); }

	Mat3 GetWorldTransform() const { return m_WorldTransform; }

	int GetLayerIndex() const { return m_LayerIndex; }

	bool HasSprite() const { return m_Sprite.get() != nullptr; }

private:
	// Call if any transform has been modified
	//
	// Also updates the transform for all children and phys body 
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

	// Updates the layer index for all children
	void UpdateLayerIndex(Entity* entity, int index);

protected:
	// Unique id of the entity; assigned during construction
	EntityId_t m_Id;


	Entity* m_Parent;
	// Points to the head of the children list
	Entity* m_Children;
	// Points to the next sibling of its parent's children list
	Entity* m_Sibling; 

	// Index of the layer that the entity is in; -1 if entity is not in the 
	// scene
	int m_LayerIndex;

	// Transform obtained from the m_LocalPosition and m_Rotation
	Mat3 m_LocalTransform;

	// Cached world transform
	//
	// Will be re-calculated if the local transform changes, or the parent's
	// world transform is changed
	Mat3 m_WorldTransform;


	// Pointer to sprite if present
	SharedPtr<Sprite> m_Sprite;


	// Pointer to phys body if present
	PhysBody* m_Body;


	// Position and rotation relative to parent
	Vec2 m_Position;
	float m_Rotation; // Rotation in radians
};

//--------------------------------------------------
//
// TypeInfo
//
// Contains metadata for each entity class
//
//--------------------------------------------------
class TypeInfo {
	friend class Entity;

public:
	TypeInfo(const char* classname, Entity* (*createInstanceFunc)(), void (*destroyInstanceFunc)(Entity*), void
			(Entity::*spawnFunc)());

	// Pointers to entity functions
	Entity* (*CreateInstance)();
	void (*DestroyInstance)(Entity*);
	void (Entity::*Spawn)();

	const char* GetClassname() { return m_Classname; }

private:
	const char* m_Classname;
};

#endif