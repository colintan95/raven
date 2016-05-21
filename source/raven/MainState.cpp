#include "MainState.h"

#include "resource/image/PngReader.h"

#include "render/TextureRegistry.h"


MainState::MainState(GameEngine* engine): BaseGameState(engine),
m_ProjectileArray(kProjectileMax)
{
	m_EntityManagerPtr = engine->GetEntityManager();
	m_PhysWorldPtr = engine->GetPhysWorld();

	// Set the game controller
	m_Controller = new GameController(this);

	memset((void*)m_PlayerDirState, 0, sizeof(bool) * 4);


	// Creates the player entity
	m_Player = (Player*)m_EntityManagerPtr->CreateEntity("Player");
	

	// Creates the player sprite
	Texture* playerTex = m_EnginePtr->GetTextureRegistry()->GetTexture("metalslug.png");
	Sprite* playerSprite = m_EntityManagerPtr->CreateSprite((Entity*)m_Player, playerTex, 1);
	playerSprite->AddClip("idle", 5, 0, 30, 40, 1, 1, 1);
	playerSprite->PlayClip("idle", true);
	playerSprite->SetOrigin(Vec2(15, 20));


	// Create the player phys body
	PhysBody* playerBody = m_EntityManagerPtr->CreatePhysBody((Entity*)m_Player, kPhysBodyControlled, kPhysLayerFriend);
	playerBody->SetOrigin(Vec2(15, 20));
	playerBody->SetWidth(30.0);
	playerBody->SetHeight(40.0);


	// Creates the enemy entity
	m_Enemy = (Enemy*)m_EntityManagerPtr->CreateEntity("Enemy");
	m_Enemy->TranslateTo(Vec2(200.0, 0.0));
	

	// Creates the enemy sprite
	Texture* enemyTex = m_EnginePtr->GetTextureRegistry()->GetTexture("ms_enemy.png");
	Sprite* enemySprite = m_EntityManagerPtr->CreateSprite((Entity*)m_Enemy, enemyTex, 1);
	enemySprite->AddClip("idle", 0, 4, 30, 40, 1, 1, 1);
	enemySprite->PlayClip("idle", true);
	enemySprite->SetOrigin(Vec2(15, 20));


	// Create the enemy phys body
	PhysBody* enemyBody = m_EntityManagerPtr->CreatePhysBody((Entity*)m_Enemy, kPhysBodyStatic, kPhysLayerEnemy);
	enemyBody->SetOrigin(Vec2(15, 20));
	enemyBody->SetWidth(30.0);
	enemyBody->SetHeight(40.0);


	m_PhysWorldPtr->ResetAllLayerIgnore();

	// Sets which layer ignores which
	m_PhysWorldPtr->AddLayerIgnore(kPhysLayerFriend, kPhysLayerFriend);
	m_PhysWorldPtr->AddLayerIgnore(kPhysLayerEnemy, kPhysLayerEnemy);
	m_PhysWorldPtr->AddLayerIgnore(kPhysLayerFriendProj, kPhysLayerFriendProj);
	m_PhysWorldPtr->AddLayerIgnore(kPhysLayerEnemyProj, kPhysLayerEnemyProj);
	m_PhysWorldPtr->AddLayerIgnore(kPhysLayerFriend, kPhysLayerEnemy);
	m_PhysWorldPtr->AddLayerIgnore(kPhysLayerFriend, kPhysLayerFriendProj);
	m_PhysWorldPtr->AddLayerIgnore(kPhysLayerEnemy, kPhysLayerEnemyProj);


	m_FireReady = true;

	//delete[] pngData;
}

MainState::~MainState() {
	m_EntityManagerPtr->DestroyEntity((Entity*)m_Enemy);
	m_EntityManagerPtr->DestroyEntity((Entity*)m_Player);

	delete m_Controller;

	m_PhysWorldPtr = nullptr;
	m_EntityManagerPtr = nullptr;
}

void MainState::Update() {
	// Moves the player in the specified direction
	if (m_PlayerDirState[kPlayerDirLeft]) {
		m_Player->TranslateBy(Vec2(-5.0, 0.0));
	}
	else if (m_PlayerDirState[kPlayerDirRight]) {
		m_Player->TranslateBy(Vec2(5.0, 0.0));
	}
	else if (m_PlayerDirState[kPlayerDirUp]) {
		m_Player->TranslateBy(Vec2(0.0, -5.0));
	}
	else if (m_PlayerDirState[kPlayerDirDown]) {
		m_Player->TranslateBy(Vec2(0.0, 5.0));
	}

	for (int i = 0; i < m_ProjectileArray.GetSize(); ++i) {
		Rect screenRect = m_EnginePtr->GetScene()->GetScreenRect();

		Projectile* projectile = m_ProjectileArray[i];

		Vec2 pos = projectile->GetWorldPosition();

		if (pos.GetX() < screenRect.GetX() ||
			pos.GetX() > (screenRect.GetX() + screenRect.GetW()) ||
			pos.GetY() < screenRect.GetY() ||
			pos.GetY() > (screenRect.GetY() + screenRect.GetH())) {

			LOG_PRINT("removed");
		}
	}
}

void MainState::PreState() {
	// Sets the game controller
	m_EnginePtr->GetInputManager()->SetGameController(m_Controller);

	// Adds the player to the scene
	m_EnginePtr->GetScene()->AddEntity(m_Player, 0);
	m_EnginePtr->GetScene()->AddEntity(m_Enemy, 0);
}

void MainState::PostState() {
	m_EnginePtr->GetScene()->RemoveEntity(m_Enemy);
	m_EnginePtr->GetScene()->RemoveEntity(m_Player);
}

void MainState::Pause() {

}

void MainState::Resume() {

}

void MainState::PlayerAttack() {
	LOG_PRINT("Attack");
}

void MainState::SetPlayerDirection(PlayerDirection_t direction, bool state) {
	m_PlayerDirState[direction] = state;

	// Flips the player in the y-axis if moving to the left
	if (direction == kPlayerDirLeft && state) {
		m_Player->FlipY(true);
	}
	else if (direction == kPlayerDirRight && state) {
		m_Player->FlipY(false);
	}
}

void MainState::FireProjectile() {
	if (!m_FireReady) {
		return;
	}

	// (TODO:) manage projectiles; they are currently causing memory leaks
	
	Projectile* projectile = (Projectile*)m_EnginePtr->GetEntityManager()->CreateEntity("Projectile");
	projectile->TranslateTo(m_Player->GetWorldPosition());


	Texture* tex = m_EnginePtr->GetTextureRegistry()->GetTexture("projectile.png");
	Sprite* sprite = m_EntityManagerPtr->CreateSprite((Entity*)projectile, tex, 1);
	sprite->AddClip("idle", 0, 0, 30, 5, 1, 1, 1);
	sprite->SetOrigin(Vec2(0, 3));


	PhysBody* body = m_EntityManagerPtr->CreatePhysBody((Entity*)projectile, kPhysBodyDynamic, kPhysLayerFriendProj);
	body->SetOrigin(Vec2(0, 3));
	body->SetWidth(30.0);
	body->SetHeight(5.0);


	if (m_Player->GetFlipY()) {
		projectile->FlipY(true);
		body->SetVelocity(Vec2(-1.0, 0.0));
	}
	else {
		projectile->FlipY(false);
		body->SetVelocity(Vec2(1.0, 0.0));
	}

	m_EnginePtr->GetScene()->AddEntity(projectile, 0);

	// Adds the projectile to the projectile array
	m_ProjectileArray.PushBack(projectile);

	m_FireReady = false;
}

void MainState::ResetProjectile() {
	m_FireReady = true;
}