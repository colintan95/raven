#include "PlatformFileSystem.h"

#if defined(_PLATFORM_WIN) 

#elif defined(_PLATFORM_OSX)
	#include "osx/PlatformFileSystemImpl_osx.h"
#elif defined(_PLATFORM_LINUX)

#endif

PlatformFileSystem::PlatformFileSystem() {
	m_Impl = new PlatformFileSystemImpl;
}

PlatformFileSystem::~PlatformFileSystem() {
	delete m_Impl;
}

bool PlatformFileSystem::AsyncRead(const PlatformFile& file, size_t offset, void* buffer, size_t size) {
	return m_Impl->AsyncRead(file, offset, buffer, size);
}

bool PlatformFileSystem::AsyncWrite(const PlatformFile& file, size_t offset, void* buffer, size_t size) {
	return m_Impl->AsyncWrite(file, offset, buffer, size);
}

void PlatformFileSystem::AsyncCancel(const PlatformFile& file) {
	m_Impl->AsyncCancel(file);
}

bool PlatformFileSystem::AsyncComplete(int* status, size_t* size) {
	return m_Impl->AsyncComplete(status, size);
}

bool PlatformFileSystem::AsyncInProgress() const {
	return m_Impl->AsyncInProgress();
}