#include "ResourceManager.h"

#include "platform/PlatformFileSystem.h"

#include "ResourceAllocator.h"
#include "ResourceStream.h"

ResourceManager::ResourceManager(PlatformFileSystem* fileSys): 
m_Registry(kResourceManagerResourceMax),
m_ReqStack(128) {
	
	ASSERT(fileSys != nullptr);

	m_FileSysPtr = fileSys;

	m_Allocator = new ResourceAllocator(this);
	m_Stream = new ResourceStream(this, fileSys);

	m_Registry.Clear();
}

ResourceManager::~ResourceManager() {
	m_Registry.Clear();

	delete m_Stream;
	delete m_Allocator;

	m_FileSysPtr = nullptr;
}

void ResourceManager::Update() {
    m_Stream->Update();

    // Send the next request to load
    if (m_Stream->CanLoad() && !m_ReqStack.IsEmpty()) {
    	ResourceLoadReq req = m_ReqStack.GetFront();

    	ResourceHandle handle = GetResource(req.path);

    	// Checks if the file has already been loaded into the manager
    	if (!handle.IsValid()) {
    		bool res = m_Stream->LoadDataFromFile(req.path, req.usage);
			if (!res) {
				LOG_ERROR("ResourceManager: File \'%s\' could not be found", req.path);
			}
    	}
    	else {
    		LOG_ERROR("ResourceManager: File \'%s\' is already loaded", req.path);
    	}

		m_ReqStack.Pop();
	}
    	
    // Handle the current request if it has finished loading
    if (m_Stream->IsComplete()) {
        ResourceBufferHandle bufHandle = m_Stream->AcquireBufferHandle();

        LOG_PRINT("Memory: %llu\nSize: %llu\nError: %i", (size_t)bufHandle.GetData(), bufHandle.GetSize(), bufHandle.IsError());
        
        m_Stream->ReleaseBufferHandle(bufHandle);
    }
}

void ResourceManager::LoadResourceFromFile(const char* path, ResourceUsage_t usage) {
	ResourceLoadReq req;
	req.path = path;
	req.usage = usage;

	m_ReqStack.Push(req);
}

ResourceHandle ResourceManager::GetResource(const char* name) {
	ResourceId_t id = CreateResourceId(name);

	return GetResource(id);
}

ResourceHandle ResourceManager::GetResource(ResourceId_t id) {
	auto it = m_Registry.Find(id);

	if (it == m_Registry.End()) {
		return ResourceHandle();
	}
	else {
		return it.GetValue().CreateHandle();
	}
}

ResourceId_t ResourceManager::CreateResourceId(const char* name) {
	return HashString(name);
}