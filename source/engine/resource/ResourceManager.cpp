#include "ResourceManager.h"

#include "platform/PlatformFileSystem.h"
#include "render/TextureRegistry.h"

#include "ResourceStream.h"
#include "image/PngReader.h"

ResourceManager::ResourceManager(PlatformFileSystem* fileSys, TextureRegistry* texRegistry): 
m_Allocator(kResourceAllocatorBlockSize, kResourceAllocatorBlockCount),
m_Registry(kResourceManagerResourceMax),
m_ReqStack(128)
{
	
	ASSERT(fileSys != nullptr);

	m_FileSysPtr = fileSys;
	m_TexRegistryPtr = texRegistry;

	m_Stream = new ResourceStream(this, fileSys);

	m_Registry.Clear();
}

ResourceManager::~ResourceManager() {
	m_Registry.Clear();

	delete m_Stream;

	m_TexRegistryPtr = nullptr;
	m_FileSysPtr = nullptr;
}

void ResourceManager::Update() {
    m_Stream->Update();

    // Send the next request to load
    HandleNextRequest();
    	
    // Handle the current request if it has finished loading
    HandleRequestCompletion();
}

void ResourceManager::HandleNextRequest() {
	if (m_Stream->CanLoad() && !m_ReqStack.IsEmpty()) {
    	ResourceLoadReq req = m_ReqStack.GetFront();

    	ResourceHandle handle = GetResource(req.path);

    	// Checks if the file has already been loaded into the manager
    	if (!handle.IsValid()) {
    		bool res = m_Stream->LoadDataFromFile(req.path, req.type);
			if (!res) {
				LOG_ERROR("ResourceManager: File \'%s\' could not be found", req.path);
			}
    	}
    	else {
    		LOG_ERROR("ResourceManager: File \'%s\' is already loaded", req.path);
    	}

		m_ReqStack.Pop();
	}
}

void ResourceManager::HandleRequestCompletion() {
	if (!m_Stream->IsComplete()) {
		return;
	}

	// Locks the buffer handle
    ResourceBufferHandle bufHandle = m_Stream->AcquireBufferHandle();

    if (!bufHandle.IsError()) {
    	// Allocates and copies memory for the data loaded

    	ASSERT(!HasResource(bufHandle.GetPath()));

    	ResourceType_t type = bufHandle.GetType();

    	if (type == kResourceTypePng) {
    		// Creates a texture from the png data; png data is discarded

    		// Reads the png data

    		PngReader reader;

			reader.InitReader(bufHandle.GetData(), bufHandle.GetSize());

			ImageHeader header;
			bool headerRead = reader.ReadHeader(&header);
			ASSERT(headerRead);

			size_t pngDataSize = header.size;

			byte_t* pngData = new byte_t[pngDataSize];

			bool dataRead = reader.ReadData(pngData);
			ASSERT(dataRead);


			// Creates the texture with the data
			if (header.colorType == kImageColorRGBA) {
				m_TexRegistryPtr->CreateTexture(bufHandle.GetPath(), kTextureColorRGBA, header.width, header.height, (const void*)pngData);
			}
			else if (header.colorType == kImageColorRGB) {
				m_TexRegistryPtr->CreateTexture(bufHandle.GetPath(), kTextureColorRGB, header.width, header.height, (const void*)pngData);
			}


			// Stores the png resource as a resource with null data

			ResourceId_t id = CreateResourceId(bufHandle.GetPath());

			Resource res;
			res.m_Data = nullptr;

			m_Registry.Insert(id, res);

			delete[] pngData;
    	}
    	else {

    		// Stores all other types of data in a resource

    		ResourceId_t id = CreateResourceId(bufHandle.GetPath());

    		void* allocMem = m_Allocator.Alloc(bufHandle.GetSize());
    		memcpy(allocMem, (void*)bufHandle.GetData(), bufHandle.GetSize());

	    	Resource res;
	    	res.m_Data = (byte_t*)allocMem;
	    	
	    	m_Registry.Insert(id, res);
    	}
    }
    else {
    	LOG_PRINT("ResourceManager: error loading \'%s\' during async io", bufHandle.GetPath());
    	return;
    }

    // Unlocks the buffer handle
    m_Stream->ReleaseBufferHandle(bufHandle);
}

void ResourceManager::LoadResourceFromFile(const char* path, ResourceType_t type) {
	ResourceLoadReq req;
	req.path = path;
	req.type = type;

	m_ReqStack.Push(req);
}

void ResourceManager::UnloadResource(const char* path) {
	ResourceId_t id = CreateResourceId(path);

	Resource* resource = GetRawResource(id);

	if (resource == nullptr) {
		return;
	}

	m_Registry.Remove(id);
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

bool ResourceManager::HasResource(const char* name) {
	ResourceId_t id = CreateResourceId(name);

    if (GetRawResource(id) != nullptr) {
    	return true;
    }
    else {
    	return false;
    }
}

Resource* ResourceManager::GetRawResource(ResourceId_t id) {
	auto it = m_Registry.Find(id);

	if (it == m_Registry.End()) {
		return nullptr;
	}
	else {
		return &it.GetValue();
	}
}

ResourceId_t ResourceManager::CreateResourceId(const char* name) {
	return HashString(name);
}