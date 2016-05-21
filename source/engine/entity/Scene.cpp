#include "Scene.h"

#include "Entity.h"
#include "Sprite.h"

#include "math/Rect.h"
#include "platform/PlatformWindow.h"

#include "render/shaders/Shader_ScreenTextured.h"

#include <cstring>

Scene::Scene(PlatformWindow* window) {
	m_WindowPtr = window;

	m_ViewScale = 1.0;

	m_SpriteShader.InitWithSource(kShaderScreenTexturedVS, kShaderScreenTexturedFS);

	memset((void*)m_Layers, 0, sizeof(SceneLayer) * kSceneLayerMax);
}

Scene::~Scene() {
	m_WindowPtr = nullptr;
}

void Scene::Update() {
	for (int i = 0; i < kSceneLayerMax; ++i) {
		if (m_Layers[i].head != nullptr) {
			UpdateInternal(m_Layers[i].head);
		}
	}
}

void Scene::UpdateInternal(Entity* entity) {
	// Updates the entity's children first
	if (entity->m_Children != nullptr) {
		UpdateInternal(entity->m_Children);
	}

	// Updates the entity
	entity->EntityUpdate();

	if (entity->HasSprite()) {
		entity->GetSprite()->Update();
	}

	// Updates the entity's siblins after the entity
	if (entity->m_Sibling != nullptr) {
		UpdateInternal(entity->m_Sibling);
	}
}

void Scene::Render() {
	for (int i = kSceneLayerMax - 1; i >= 0; --i) {
		if (m_Layers[i].head != nullptr) {
			RenderInternal(m_Layers[i].head);
		}
	}
}

void Scene::RenderInternal(Entity* entity) {
	// Renders the entity's children first
	if (entity->m_Children != nullptr) {
		RenderInternal(entity->m_Children);
	}

	// Render the entity
	if (entity->HasSprite()) {
		Sprite* sprite = entity->GetSprite();

		// Sprite frame in world coordinates
		Rect frame = sprite->GetFrame();


		Vec2 spriteOrigin = sprite->GetOrigin();

		// Sprite coordinates
		Vec3 topLeftSprite = Vec3(	-spriteOrigin.GetX(),
									-spriteOrigin.GetY(), 1.0);
		Vec3 topRightSprite = Vec3(	-spriteOrigin.GetX() + frame.GetW(),
									-spriteOrigin.GetY(), 1.0);
		Vec3 botRightSprite = Vec3(	-spriteOrigin.GetX() + frame.GetW(),
									-spriteOrigin.GetY() + frame.GetH(), 1.0);
		Vec3 botLeftSprite = Vec3(	-spriteOrigin.GetX(),
									-spriteOrigin.GetY() + frame.GetH(), 1.0);


		Mat3 worldTransform = entity->GetWorldTransform();

		if (sprite->GetFlipY()) {
			worldTransform = worldTransform * Mat3(-1.0, 0.0, 0.0,
													0.0, 1.0, 0.0,
													0.0, 0.0, 1.0);
		}

		if (sprite->GetFlipX()) {
			worldTransform = worldTransform * Mat3(1.0, 0.0, 0.0,
													0.0, -1.0, 0.0,
													0.0, 0.0, 1.0);
		}

		// World coordinates
		Vec3 topLeftWorld = worldTransform * topLeftSprite;
		Vec3 topRightWorld = worldTransform * topRightSprite;
		Vec3 botRightWorld = worldTransform * botRightSprite;
		Vec3 botLeftWorld = worldTransform * botLeftSprite;


		// Normalized device coordinates
		Vec3 topLeftNDC = ViewToNDC(WorldToViewCoords(topLeftWorld));
		Vec3 topRightNDC = ViewToNDC(WorldToViewCoords(topRightWorld));
		Vec3 botRightNDC = ViewToNDC(WorldToViewCoords(botRightWorld));
		Vec3 botLeftNDC = ViewToNDC(WorldToViewCoords(botLeftWorld));


		double texWidth = (double)sprite->GetTexture()->GetWidth();
		double texHeight = (double)sprite->GetTexture()->GetHeight();

		// Coords of bottom left corner
		double texX = frame.GetX();
		double texY = texHeight - (frame.GetY() + frame.GetH());

		// Texcoord
		//
		// OpenGL texcoords are inverted 
		Vec2 topLeftTex = Vec2(	texX / texWidth, 
								(texY + frame.GetH()) / texHeight);
		Vec2 topRightTex = Vec2((texX + frame.GetW()) / texWidth, 
								(texY + frame.GetH()) / texHeight);
		Vec2 botRightTex = Vec2((texX + frame.GetW()) / texWidth, 
								texY / texHeight);

		Vec2 botLeftTex = Vec2(	texX / texWidth, 
								texY / texHeight);
		

		RenderQuad quad;
		quad.SetPosition(	Vec2(topLeftNDC.GetX(), topLeftNDC.GetY()), 
							Vec2(topRightNDC.GetX(), topRightNDC.GetY()), 
							Vec2(botRightNDC.GetX(), botRightNDC.GetY()), 
							Vec2(botLeftNDC.GetX(), botLeftNDC.GetY()) );

		// Sets texcoords; accounts for flipping of sprite
		/*if (sprite->GetFlipX() && sprite->GetFlipY()) {
			quad.SetTexcoord(	botRightTex,
								botLeftTex,
								topLeftTex,
								topRightTex	);
		}
		else if (sprite->GetFlipX()) {
			quad.SetTexcoord(	botLeftTex,
								botRightTex,
								topRightTex,
								topLeftTex	);
		}
		else if (sprite->GetFlipY()) {
			quad.SetTexcoord(	topRightTex,
								topLeftTex,
								botLeftTex,
								botRightTex	);
		}
		else {
			quad.SetTexcoord(	topLeftTex,
								topRightTex,
								botRightTex,
								botLeftTex	);
		}*/
		
		quad.SetTexcoord(	topLeftTex,
							topRightTex,
							botRightTex,
							botLeftTex	);


		m_SpriteShader.BindShader();
        
		m_SpriteShader.SetUniformTexture(kShaderUniformTexture2D, *(sprite->GetTexture()));
		m_SpriteShader.SetRenderQuad(quad);

		m_SpriteShader.Execute(4);

		m_SpriteShader.UnbindShader();
	}

	// Renders the entity's siblings after the entity
	if (entity->m_Sibling != nullptr) {
		RenderInternal(entity->m_Sibling);
	}
}

void Scene::AddEntity(Entity* entity, int layerIndex) {
	ASSERT(entity != nullptr);
	ASSERT(layerIndex >= 0 && layerIndex < kSceneLayerMax);

	// Only entities that are not a child of another
	ASSERT(entity->m_Parent == nullptr);

	SceneLayer* layer = &m_Layers[layerIndex];

	if (layer->head == nullptr) {
		// Adds the entity when the layer is initially empty

		layer->head = entity;
		layer->tail = entity;

		entity->m_Parent = nullptr;
		entity->m_Sibling = nullptr;
	}
	else {
		// Adds the entity when the layer has elements

		layer->tail->m_Sibling = entity;
		layer->tail = entity;

		entity->m_Parent = nullptr;
		entity->m_Sibling = nullptr;
	}

	entity->SetLayerIndex(layerIndex);
}

void Scene::RemoveEntity(Entity* entity) {
	ASSERT(entity != nullptr);

	ASSERT(entity->m_LayerIndex != -1);

	// Only entities that are not a child of another
	ASSERT(entity->m_Parent == nullptr);

	SceneLayer* layer = &m_Layers[entity->m_LayerIndex];

	Entity** prevPtr = &layer->head;
	Entity* prevIt = nullptr;
	Entity* entityIt = layer->head;

	while (entityIt != nullptr && entityIt != entity) {
		prevPtr = &(entityIt->m_Sibling);
		prevIt = entityIt;
		entityIt = entityIt->m_Sibling;
	}

	ASSERT(entityIt != nullptr);

	if (entityIt == entity) {
		*prevPtr = entityIt->m_Sibling;

		if (layer->tail == entityIt) {
			layer->tail = prevIt;
		}

		entityIt->m_Sibling = nullptr;

		entityIt->SetLayerIndex(-1);
	}
}

void Scene::MoveCameraTo(const Vec2& pt) {
	m_CameraPos = pt;
}

Vec3 Scene::WorldToViewCoords(const Vec3& pt) const {
	ASSERT(pt.GetZ() == 1);

	return Vec3(m_ViewScale * (pt.GetX() - m_CameraPos.GetX()),
				m_ViewScale * (pt.GetY() - m_CameraPos.GetY()),
				1.0	);
}

Vec3 Scene::ScreenToWorldCoords(const Vec3& pt) const {
	ASSERT(pt.GetZ() == 1);

	/*return Vec3(pt.GetX() / m_ViewScale + m_CameraPos.GetX(),
				pt.GetY() / m_ViewScale + m_CameraPos.GetY(),
				1.0 );*/

	double width = m_WindowPtr->GetWindowWidth();
	double height = m_WindowPtr->GetWindowHeight();

	return Vec3(pt.GetX() / m_ViewScale + m_CameraPos.GetX() - width / 2.0,
				pt.GetY() / m_ViewScale + m_CameraPos.GetY() - height / 2.0,
				1.0 );
}

Rect Scene::GetScreenRect() const {
	Vec3 topLeft = ScreenToWorldCoords(Vec3(0.0, 0.0, 1.0));
	Vec3 botRight = ScreenToWorldCoords(Vec3(m_WindowPtr->GetWindowWidth(),
										m_WindowPtr->GetWindowHeight(),
										1.0));

	return Rect(topLeft.GetX(), topLeft.GetY(), botRight.GetX() - topLeft.GetX(), botRight.GetY() - topLeft.GetY());
}

Vec3 Scene::ViewToNDC(const Vec3& pt) {
	ASSERT(pt.GetZ() == 1);

	return Vec3((pt.GetX() * 2.0) / (double)m_WindowPtr->GetWindowWidth(),
				(pt.GetY() * -2.0) / (double)m_WindowPtr->GetWindowHeight(),
				1.0 );
}