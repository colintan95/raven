#include "GameEngine.h"

#include "resource/ResourceManager.h"
#include "input/InputManager.h"
#include "render/Renderer.h"
#include "entity/Scene.h"

#include "entity/Entity.h"
#include "entity/Sprite.h"

#include "resource/image/PngReader.h"

#include <cstdio>

GameEngine::GameEngine() {
	m_Quit = false;
}

GameEngine::~GameEngine() {

}

void GameEngine::Init() {
	// Sets up all platform specific subsystems
	// MUST be called FIRST
	m_PlatformApp = new PlatformApp();
	m_PlatformWindow = new PlatformWindow();
	m_PlatformInput = new PlatformInput(m_PlatformWindow);

	m_PlatformFileSys = new PlatformFileSystem();

	m_Resource = new ResourceManager(m_PlatformFileSys);
	m_Input = new InputManager(this, m_PlatformInput);
	m_Render = new Renderer(m_PlatformWindow);

	m_Scene = new Scene(m_PlatformWindow);
}

void GameEngine::Shutdown() {
	delete m_Scene;

	delete m_Render;
	delete m_Input;
	delete m_Resource;

	delete m_PlatformFileSys;
	delete m_PlatformInput;
	delete m_PlatformWindow;
	delete m_PlatformApp;
}

void GameEngine::MainLoop() {

	FILE* pngFile = fopen("metalslug.png", "rb");

	fseek(pngFile, 0, SEEK_END);
	int pngFileSize = ftell(pngFile);
	fseek(pngFile, 0, SEEK_SET);

	byte_t* pngBuffer = new byte_t[pngFileSize];

	fread((void*)pngBuffer, 1, pngFileSize, pngFile);

	fclose(pngFile);


	PngReader reader;

	reader.InitReader(pngBuffer, pngFileSize);

	ImageHeader header;
	bool headerRead = reader.ReadHeader(&header);

	ASSERT(headerRead);

	size_t pngDataSize = header.size;

	byte_t* pngData = new byte_t[pngDataSize];

	bool dataRead = reader.ReadData(pngData);

	ASSERT(dataRead);

	delete[] pngBuffer;

	Texture ballTexture;

	ballTexture.CreateFromBuffer(kTextureColorRGBA, header.width, header.height, (const void*)pngData);
	

	SharedPtr<Sprite> ballSprite(new Sprite(&ballTexture, 1));

	ballSprite->AddClip("idle", 5, 0, 30, 40, 1, 1, 1);
	ballSprite->PlayClip("idle", true);

	Entity* ballEntity = new Entity();
	ballEntity->SetSprite(ballSprite);

	m_Scene->AddEntity(ballEntity, 0);

	while (!m_Quit) {
		// Update the resource manager;
		m_Resource->Update();

		// Update input
		m_Input->Update();

		// Update scene
		m_Scene->Update();


		// Rendering logic

		// Sets up before any rendering
		m_Render->PreRender();

		//m_Render->Render();

		m_Scene->Render();

		m_Render->PostRender();
	}

	delete ballEntity;

	delete[] pngData;
}

void GameEngine::QuitGame() {
	m_Quit = true;
}