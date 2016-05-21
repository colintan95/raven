#include "EntityManager.h"

#include "physics/PhysWorld.h"
#include "Entity.h"

// Definition of type table
HashMap<const char*, TypeInfo*> EntityManager::m_TypeTable(kEntityTypeMax);

void EntityManager::AddType(const char* name, TypeInfo* type) {
	m_TypeTable.Insert(name, type);
}

EntityManager::EntityManager(PhysWorld* physWorld) {
	m_PhysWorldPtr = physWorld;
}

EntityManager::~EntityManager() {
	m_PhysWorldPtr = nullptr;
}

Entity* EntityManager::CreateEntity(const char* classname) {
	auto it = m_TypeTable.Find(classname);

	if (it == m_TypeTable.End()) {
		LOG_ERROR("EntityManager: class \'%s\' could not be found", classname);
		return nullptr;
	}

	TypeInfo* type = it.GetValue();

	Entity* entity = (*(type->CreateInstance))();

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity) {
	PhysBody* body = entity->m_Body;

	if (body != nullptr) {
		m_PhysWorldPtr->DestroyBody(body);
		entity->m_Body = nullptr;
	}

	TypeInfo* type = entity->GetType();

	(*(type->DestroyInstance))(entity);
}

Sprite* EntityManager::CreateSprite(Entity* entity, Texture* texture, int clipCount) {
	ASSERT(entity->m_Sprite.get() == nullptr);

	SharedPtr<Sprite> sprite(new Sprite(texture, clipCount));

	entity->m_Sprite = sprite;

	return sprite.get();
}

PhysBody* EntityManager::CreatePhysBody(Entity* entity, PhysBodyType_t type, uint16_t layer) {
	ASSERT(entity != nullptr);
	ASSERT(entity->m_Body == nullptr);

	PhysBody* body = m_PhysWorldPtr->CreateBody(type, layer, entity);

	entity->m_Body = body;
    
    return body;
}