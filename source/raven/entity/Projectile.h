#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "base_include.h"

#include "entity/Entity.h"

class Projectile: public Entity {
	CONCRETE_PROTOTYPE(Projectile)

public:
	Projectile();
	virtual ~Projectile();

	virtual void Spawn();

	virtual void Update();

	virtual void OnCollision(Entity* entity);

private:

};

#endif