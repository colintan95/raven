#ifndef PLAYER_H_
#define PLAYER_H_

#include "base_include.h"

#include "entity/Entity.h"

class Player: public Entity {
	CONCRETE_PROTOTYPE(Player)

public:
	Player();
	virtual ~Player();

	virtual void Spawn();

	virtual void Update();

	virtual void OnCollision(Entity* entity);

private:

};

#endif