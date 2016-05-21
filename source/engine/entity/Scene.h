#ifndef SCENE_H_
#define SCENE_H_

#include "base_include.h"

#include "math/Rect.h"
#include "math/Vector.h"
#include "render/QuadShader.h"

const int kSceneLayerMax = 16;

// Forward declarations
class Entity;

struct SceneLayer {
	Entity* head;
	Entity* tail;
};

// Forward declarations
class PlatformWindow;

//--------------------------------------------------
//
// Scene
//
// Contains and handles all entities in the game world
//
//--------------------------------------------------
class Scene {

public:
	Scene(PlatformWindow* window);
	~Scene();

	// Update the entities on each frame
	void Update();

	// Render all entities with sprites
	void Render();
	

	// Only add or remove entity that is not a child to another entity
	// (ie m_Parent == nullptr)
	void AddEntity(Entity* entity, int layerIndex);
	void RemoveEntity(Entity* entity);

	// Moves the entity in front of its next sibling
	void MoveForward(Entity* entity);

	// Moves the entity behind the previous sibling
	void MoveBackward(Entity* entity);

	// Moves the entity to the front of the layer
	void MoveToFront(Entity* entity);

	// Moves the entity to the back of the layer
	void MoveToBack(Entity* entity);


	void MoveCameraTo(const Vec2& vec);


	Vec3 WorldToViewCoords(const Vec3& pt) const;

	Vec3 ScreenToWorldCoords(const Vec3& pt) const;

	// Returns the screen rect in world coordinates
	Rect GetScreenRect() const;

private:
	// Converts view coordinates to Normalized Device Coordinates
	Vec3 ViewToNDC(const Vec3& pt);

private:
	// Recursive function called by Update()
	void UpdateInternal(Entity* entity);

	// Recursive function called by Render()
	void RenderInternal(Entity* entity);

private:
	PlatformWindow* m_WindowPtr;

	QuadShader m_SpriteShader;


	// Layer precedence is in ascending order (0 is the layer closest to the 
	// player)
	//
	// Entities nearer to the tail of the layer are closer to the player
	SceneLayer m_Layers[kSceneLayerMax];

	// Location of the camera in the scene
	Vec2 m_CameraPos;

	// Factor that the scene is scaled by when viewed by the player
	double m_ViewScale;
};

#endif