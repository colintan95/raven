#include "Entity.h"

// Tracks the next entity id to assign
static EntityId_t entityIdCounter = 1;

Entity::Entity() {
	m_Id = entityIdCounter;
	++entityIdCounter;

	m_Parent = nullptr;
	m_Children = nullptr;
	m_Sibling = nullptr;

	m_Position = Vec2(0.0, 0.0);
	m_Rotation = 0.0;

	UpdateAllTransform();
}

Entity::~Entity() {
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

void Entity::Update() {

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