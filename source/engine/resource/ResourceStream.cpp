#include "ResourceStream.h"

#include "platform/PlatformFileSystem.h"

#include "ResourceManager.h"

//--------------------------------------------------
//
// ResourceBufferHandle
//
//--------------------------------------------------
ResourceBufferHandle::ResourceBufferHandle() {
	m_StreamPtr = nullptr;
	m_Buffer = nullptr;
	m_Size = 0;
	m_Error = false;
	m_Usage = kResourceSingleUse;
}

ResourceBufferHandle::ResourceBufferHandle(ResourceStream* stream, byte_t* buffer, size_t size, bool error, ResourceUsage_t usage) {
	ASSERT(stream != nullptr);
	ASSERT(buffer != nullptr);
	ASSERT(size > 0);

	m_StreamPtr = stream;
	m_Buffer = buffer;
	m_Size = size;
	m_Error = error;
	m_Usage = usage;
}

ResourceBufferHandle::~ResourceBufferHandle() {
	m_StreamPtr = nullptr;
	m_Buffer = nullptr;
	m_Size = 0;
	m_Error = false;
}


//--------------------------------------------------
//
// ResourceStream
//
//--------------------------------------------------
ResourceStream::ResourceStream(ResourceManager* manager, PlatformFileSystem* fileSys) {

	ASSERT(manager != nullptr);
	ASSERT(fileSys != nullptr);

	m_ManagerPtr = manager;
	m_FileSysPtr = fileSys;

	// Initialize the two buffers
	m_Buffers[0].data = new byte_t[kResourceLoadSizeMax];
	m_Buffers[1].data = new byte_t[kResourceLoadSizeMax];
	m_Buffers[0].status = kResourceBufferEmpty;
	m_Buffers[1].status = kResourceBufferEmpty;
	m_Buffers[0].size = 0;
	m_Buffers[1].size = 0;
	m_Buffers[0].error = false;
	m_Buffers[1].error = false;

	m_FrontBuffer = &m_Buffers[0];
	m_BackBuffer = &m_Buffers[1];
}

ResourceStream::~ResourceStream() {
	if (m_FileSysPtr->AsyncInProgress()) {
		m_FileSysPtr->AsyncCancel(m_File);
	}

	if (m_File.IsOpen()) {
		m_File.Close();
	}

	m_FrontBuffer = nullptr;
	m_BackBuffer = nullptr;

	delete[] m_Buffers[0].data;
	delete[] m_Buffers[1].data;
	m_Buffers[0].status = kResourceBufferEmpty;
	m_Buffers[1].status = kResourceBufferEmpty;
	m_Buffers[0].size = 0;
	m_Buffers[1].size = 0;
	m_Buffers[0].error = false;
	m_Buffers[1].error = false;

	m_FileSysPtr = nullptr;
	m_ManagerPtr = nullptr;
}

void ResourceStream::Update() {
	CheckForCompletion();
}

bool ResourceStream::LoadDataFromFile(const char* path, ResourceUsage_t usage) {
	ASSERT(!m_File.IsOpen());
	ASSERT(CanLoad());

	if (!CanLoad()) {
		return false;
	}
	
	if (m_FileSysPtr->AsyncInProgress()) {
		return false;
	}

	bool openRes = m_File.Open(path, kPlatformFileRead);

	if (!openRes) {
		return false;
	}

	m_FileSysPtr->AsyncRead(m_File, 0, m_BackBuffer->data, kResourceLoadSizeMax);

	m_BackBuffer->status = kResourceBufferProgress;

	m_BackBuffer->usage = usage;

	return true;
}

bool ResourceStream::CanLoad() const {
	return (m_BackBuffer->status == kResourceBufferEmpty);
}

bool ResourceStream::IsComplete() const {
	return (m_FrontBuffer->status == kResourceBufferWaiting);
}

ResourceBufferHandle ResourceStream::AcquireBufferHandle() {
	ASSERT(IsComplete());

	if (!IsComplete()) {
        ResourceBufferHandle handle;
        
        return handle;
	}
    
    ResourceBufferHandle handle(this, m_FrontBuffer->data, m_FrontBuffer->size, m_FrontBuffer->error, m_FrontBuffer->usage);

	m_FrontBuffer->status = kResourceBufferProgress;

	return handle;
}

void ResourceStream::ReleaseBufferHandle(ResourceBufferHandle& handle) {
	ASSERT(m_FrontBuffer->status == kResourceBufferProgress);

	m_FrontBuffer->status = kResourceBufferEmpty;

	if (m_BackBuffer->status == kResourceBufferWaiting) {
		SwapBuffers();
	}

	handle = ResourceBufferHandle();
}

void ResourceStream::CheckForCompletion() {

	int asyncStatus = 0;
	size_t asyncSize = 0;

	bool asyncRes = m_FileSysPtr->AsyncComplete(&asyncStatus, &asyncSize);
    
    if (asyncRes == false) {
        return;
    }

	if (asyncStatus == kAsyncStatusSuccess) {
		m_BackBuffer->error = false;
	}
	else if (asyncStatus == kAsyncStatusError) {
		m_BackBuffer->error = true;
	}
	else {
		return;
	}

	m_BackBuffer->status = kResourceBufferWaiting;
	m_BackBuffer->size = asyncSize;

	m_File.Close();

	if (m_FrontBuffer->status == kResourceBufferEmpty) {
		SwapBuffers();
	}
}

void ResourceStream::SwapBuffers() {
	ASSERT(m_FrontBuffer->status == kResourceBufferEmpty);
	ASSERT(m_BackBuffer->status == kResourceBufferWaiting);

	ResourceBuffer* tmp = m_FrontBuffer;
	m_FrontBuffer = m_BackBuffer;
	m_BackBuffer = tmp;
}