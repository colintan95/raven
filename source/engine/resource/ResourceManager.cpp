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
    	const char* path = m_ReqStack.GetFront();
		bool res = m_Stream->LoadDataFromFile(path);
		if (!res) {
			LOG_ERROR("ResourceManager: File \'%s\' could not be found", path);
		}
		m_ReqStack.Pop();
	}
    	
    // Handle the current request if it has finished loading
    if (m_Stream->IsComplete()) {
        ResourceBufferHandle handle = m_Stream->AcquireBufferHandle();

        //LOG_PRINT("Memory: %llu\nSize: %llu\nError: %i", (size_t)handle.GetData(), handle.GetSize(), handle.IsError());
        
        m_Stream->ReleaseBufferHandle(handle);
    }
}

void ResourceManager::LoadResourceFromFile(const char* path) {
	m_ReqStack.Push(path);
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