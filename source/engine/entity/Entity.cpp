#include "Entity.h"

#include "EntityManager.h"
#include "Sprite.h"
#include "physics/PhysBody.h"

// Tracks the next entity id to assign
static EntityId_t entityIdCounter = 1;

//--------------------------------------------------
//
// Entity
//
//--------------------------------------------------
ABSTRACT_DECLARATION(Entity);

Entity::Entity() {
	m_Id = entityIdCounter;
	++entityIdCounter;

	m_Parent = nullptr;
	m_Children = nullptr;
	m_Sibling = nullptr;

	m_LayerIndex = -1;

	m_Position = Vec2(0.0, 0.0);
	m_Rotation = 0.0;

	m_Body = nullptr;

	UpdateAllTransform();
}

Entity::~Entity() {
	ASSERT(m_Body == nullptr);

	if (m_Parent != nullptr) {
		m_Parent->RemoveChild(this);
	}

	while(m_Children != nullptr) {
		this->RemoveChild(m_Children);
	}

	m_Parent = nullptr;
	m_Children = nullptr;
	m_Sibling = nullptr;

	m_Id = 0;
}

void Entity::EntityUpdate() {
	Update();
}

void Entity::TranslateBy(const Vec2& vec) {
	m_Position += vec;

	UpdateAllTransform(); // MUST be called 
}

void Entity::RotateBy(double rotation) {
	m_Rotation += rotation;

	UpdateAllTransform(); // MUST be called 
}

void Entity::TranslateTo(const Vec2& vec) {
	m_Position = vec;

	UpdateAllTransform(); // MUST be called 
}

void Entity::RotateTo(double rotation) {
	m_Rotation = rotation;

	UpdateAllTransform(); // MUST be called
}

void Entity::UpdateAllTransform() {
	CalcLocalTransform();
	
	CalcWorldAndChildrenTransform();

	if (m_Body != nullptr) {
		m_Body->UpdateFromEntity();
	}
}

void Entity::CalcLocalTransform() {
	m_LocalTransform = Mat3::CreateTranslateMat(m_Position.GetX(), m_Position.GetY()) * Mat3::CreateRotateMat(m_Rotation);
}

void Entity::CalcWorldAndChildrenTransform() {
	if (m_Parent != nullptr) {
		m_WorldTransform = m_Parent->m_WorldTransform * m_LocalTransform;
	}
	else {
		m_WorldTransform = m_LocalTransform;
	}

	// Calls CalcWorldAndChildrenTransform() for all children to update their 
	// world transforms
	Entity* it = m_Children;
	while (it != nullptr) {
		it->CalcWorldAndChildrenTransform();
		it = it->m_Sibling;
	}
}

void Entity::FlipX(bool flag) {
	if (m_Sprite != nullptr) {
		m_Sprite->SetFlipX(flag);
	}
}

void Entity::FlipY(bool flag) {
	if (m_Sprite != nullptr) {
		m_Sprite->SetFlipY(flag);
	}
}

void Entity::AddChild(Entity* entity) {
	ASSERT(entity != nullptr);
	ASSERT(entity->m_Sibling == nullptr);

	Entity** prevPtr = &m_Children;
	Entity* it = m_Children;

	while (it != nullptr) {
		if (it == entity) {
			LOG_WARNING("Entity %llu is already a child of Entity %llu", entity->m_Id, this->m_Id);
			return;
		}

		prevPtr = &(it->m_Sibling);
		it = it->m_Sibling;
	}

	*prevPtr = entity;
	entity->m_Parent = this;
	entity->m_Sibling = nullptr;

	entity->m_LayerIndex = m_LayerIndex;
}

void Entity::RemoveChild(Entity* entity) {
	ASSERT(entity != nullptr);
	ASSERT(entity->m_Parent == this);

	Entity** prevPtr = &m_Children;
	Entity* it = m_Children;

	while (it != nullptr) {
		if (it == entity) {
			*prevPtr = it->m_Sibling;
			it->m_Parent = nullptr;
			it->m_Sibling = nullptr;
			return;
		}

		prevPtr = &(it->m_Sibling);
		it = it->m_Sibling;
	}

	LOG_WARNING("Entity %llu is not a child of Entity %llu", entity->m_Id, this->m_Id);
}

void Entity::SetLayerIndex(int index) {
	m_LayerIndex = index;

	if (m_Children != nullptr) {
		UpdateLayerIndex(m_Children, index);
	}
}

void Entity::UpdateLayerIndex(Entity* entity, int index) {
	if (entity->m_Children != nullptr) {
		entity->m_Children->SetLayerIndex(index);
	}

	entity->m_LayerIndex = index;

	if (entity->m_Sibling != nullptr) {
		entity->m_Sibling->SetLayerIndex(index);
	}
}

Vec2 Entity::GetWorldPosition() const {
	Vec3 res = GetWorldTransform() * Vec3(0.0, 0.0, 1.0);

	return Vec2(res.GetX(), res.GetY());
}

bool Entity::GetFlipX() const { 
	return m_Sprite->GetFlipX();
}

bool Entity::GetFlipY() const {
	return m_Sprite->GetFlipY();
}

//--------------------------------------------------
//
// TypeInfo
//
//--------------------------------------------------
TypeInfo::TypeInfo(const char* classname, Entity* (*createInstanceFunc)(), void (*destroyInstanceFunc)(Entity*), void (Entity::*spawnFunc)()) {
	m_Classname = classname;

	this->CreateInstance = createInstanceFunc;
    this->DestroyInstance = destroyInstanceFunc;
	this->Spawn = spawnFunc;

	EntityManager::AddType(classname, this);
}
