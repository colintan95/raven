#include "PhysBody.h"

#include "entity/Entity.h"

PhysBody::PhysBody() {
	m_Type = kPhysBodyNone;
	m_EntityPtr = nullptr;

	m_Layer = 0;

	m_Origin = Vec2(0.0, 0.0);
	m_Width = 0.0;
	m_Height = 0.0;
}

PhysBody::PhysBody(PhysBodyType_t type, uint16_t layer, Entity* entity) {
	m_Type = type;
	m_EntityPtr = entity;

	m_Layer = layer;

	m_Origin = Vec2(0.0, 0.0);
	m_Width = 0.0;
	m_Height = 0.0;

	UpdateFromEntity();
}

PhysBody::~PhysBody() {
	m_EntityPtr = nullptr;
}

void PhysBody::Update() {
	if (m_Velocity != Vec2(0.0, 0.0)) {
		TranslateBy(m_Velocity);
	}
}

void PhysBody::TranslateBy(const Vec2& vec) {
	m_EntityPtr->TranslateBy(vec);

	UpdateFromEntity();
}

void PhysBody::TranslateTo(const Vec2& vec) {
	m_EntityPtr->TranslateTo(vec);

	UpdateFromEntity();
}

void PhysBody::UpdateFromEntity() {
	m_Transform = m_EntityPtr->GetWorldTransform();
}