#ifndef ENEMY_H_
#define ENEMY_H_

#include "base_include.h"

#include "entity/Entity.h"

class Enemy: public Entity {
	CONCRETE_PROTOTYPE(Enemy)

public:
	Enemy();
	virtual ~Enemy();

	virtual void Spawn();

	virtual void Update();

	virtual void OnCollision(Entity* entity);

private:

};

#endif