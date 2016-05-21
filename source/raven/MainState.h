#ifndef MAINSTATE_H_
#define MAINSTATE_H_

#include "base_include.h"

#include "container/DynArray.h"

#include "state/BaseGameState.h"
#include "GameEngine.h"

#include "GameController.h"

#include "entity/EntityManager.h"
#include "entity/Sprite.h"

#include "entity/Player.h"
#include "entity/Enemy.h"
#include "entity/Projectile.h"


enum PlayerDirection_t {
	kPlayerDirUp = 0,
	kPlayerDirDown = 1,
	kPlayerDirLeft = 2,
	kPlayerDirRight = 3
};

const int kProjectileMax = 128;

// Physics layers
const uint16_t kPhysLayerFriend = 1;
const uint16_t kPhysLayerEnemy = 2;
const uint16_t kPhysLayerProps = 3;
const uint16_t kPhysLayerFriendProj = 4; // Friendly projectiles
const uint16_t kPhysLayerEnemyProj = 5; // Enemy projectiles

//--------------------------------------------------
//
// MainState
//
// Main game logic
//
//--------------------------------------------------
class MainState: public BaseGameState {

public:
	MainState(GameEngine* engine);
	virtual ~MainState();

	virtual void Update();

	virtual void PreState();
	virtual void PostState();
	
	virtual void Pause();
	virtual void Resume();

	void PlayerAttack();

	// Sets the player direction state to the specified state
	//
	// If the direction is state is true, the player will move in then 
	// direction
	// Player will only move in one direction at a time in this priority (
	// left, right, up, down)
	void SetPlayerDirection(PlayerDirection_t direction, bool state);

	// FireProjectile() only fires when the fire key is pressed once
	//
	// Call ResetProjectile() when fire key is up to allow the next projectile 
	// to fire
	void FireProjectile();
	void ResetProjectile();

private:
	GameController* m_Controller;

	EntityManager* m_EntityManagerPtr;
	PhysWorld* m_PhysWorldPtr;

	Player* m_Player;
	Enemy* m_Enemy;

	DynArray<Projectile*> m_ProjectileArray;
	
	// True for the corresponding direction if the key is down
	bool m_PlayerDirState[4];


	// Variables for firing projectiles
	bool m_FireReady;
};

#endif