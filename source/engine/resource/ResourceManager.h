#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include "base_include.h"

#include "container/HashMap.h"
#include "container/Stack.h"

#include "allocator/BlockAllocator.h"

#include "Resource.h"
#include "ResourceHandle.h"

// Max number of resources that can be stored in resource manager
const int kResourceManagerResourceMax = 1024;

const int kResourceAllocatorBlockSize = MEGABYTES_TO_BYTES(2);
const int kResourceAllocatorBlockCount = 128;

// Forward declarations
class PlatformFileSystem;
class TextureRegistry;
class ResourceStream;

// Resource load request
struct ResourceLoadReq {
	const char* path;
	ResourceType_t type;
};

//--------------------------------------------------
//
// ResourceManager
//
// Controls loading, storing and access of all resources in the game
//
//--------------------------------------------------
class ResourceManager {

public:
	ResourceManager(PlatformFileSystem* filesys, TextureRegistry* texRegistry);
	~ResourceManager();

	// Called on each frame
	void Update();

	void LoadResourceFromFile(const char* path, ResourceType_t type);

	void UnloadResource(const char* path);

	ResourceHandle GetResource(const char* name);
	ResourceHandle GetResource(ResourceId_t id);

	bool HasResource(const char* name);

private:
	void HandleNextRequest();
	void HandleRequestCompletion();

	Resource* GetRawResource(ResourceId_t id);

	// Uses a hash function to create the id
	ResourceId_t CreateResourceId(const char* name);

private:
	// Pointer to the platform file system
	PlatformFileSystem* m_FileSysPtr;

	TextureRegistry* m_TexRegistryPtr;

	// Components
	BlockAllocator m_Allocator;
	ResourceStream* m_Stream;

	// Contains all resources, indexed by resource id
	HashMap<ResourceId_t, Resource> m_Registry;

	Stack<ResourceLoadReq> m_ReqStack;
};

#endif