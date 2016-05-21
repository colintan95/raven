#ifndef ENTITYMANAGER_H_
#define ENTITYMANAGER_H_

#include "base_include.h"

#include "container/HashMap.h"

#include "entity/Sprite.h"
#include "physics/PhysWorld.h"

// Max number of subclasses of entity
const int kEntityTypeMax = 64;

// Max number of entities
const int kEntityCountMax = 1024;

// Forward declarations
class TypeInfo;
class Entity;

class EntityManager {

public:
	static void AddType(const char* name, TypeInfo* type);

public:
	EntityManager(PhysWorld* physWorld);
	~EntityManager();

	Entity* CreateEntity(const char* classname);

	// Function will destroy any PhysBody attached to the entity
	void DestroyEntity(Entity* entity);

	Sprite* CreateSprite(Entity* entity, Texture* texture, int clipCount);

	// Creates and attaches a PhysBody to the specified entity
	PhysBody* CreatePhysBody(Entity* entity, PhysBodyType_t type, uint16_t layer);

private:
	static HashMap<const char*, TypeInfo*> m_TypeTable;

	PhysWorld* m_PhysWorldPtr;
};

#endif