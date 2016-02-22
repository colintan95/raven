#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include "base_include.h"

#include "container/HashMap.h"
#include "container/Stack.h"

#include "Resource.h"
#include "ResourceHandle.h"

// Max number of resources that can be stored in resource manager
const int kResourceManagerResourceMax = 1024;

// Forward declarations
class PlatformFileSystem;
class ResourceAllocator;
class ResourceStream;

//--------------------------------------------------
//
// ResourceManager
//
// Controls loading, storing and access of all resources in the game
//
//--------------------------------------------------
class ResourceManager {

public:
	ResourceManager(PlatformFileSystem* filesys);
	~ResourceManager();

	// Called on each frame
	void Update();

	void RequestResourceFromFile(const char* path, ResourceUsage_t usage);

	void LoadResourceFromFile(const char* path);

	ResourceHandle GetResource(const char* name);
	ResourceHandle GetResource(ResourceId_t id);

private:

	// Uses a hash function to create the id
	ResourceId_t CreateResourceId(const char* name);

private:
	// Pointer to the platform file system
	PlatformFileSystem* m_FileSysPtr;

	// Components
	ResourceAllocator* m_Allocator;
	ResourceStream* m_Stream;

	// Contains all resources, indexed by resource id
	HashMap<ResourceId_t, Resource> m_Registry;

	Stack<const char*> m_ReqStack;
};

#endif