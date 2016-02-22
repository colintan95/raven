#include "PlatformFileSystemImpl_osx.h"

#include "PlatformFile_osx.h"

#include "../PlatformFileSystem.h"

PlatformFileSystemImpl::PlatformFileSystemImpl() {
	m_AioInProgress = false;

	ResetAsync();
}

PlatformFileSystemImpl::~PlatformFileSystemImpl() {
	m_AioInProgress = false;

	ResetAsync();
}

bool PlatformFileSystemImpl::AsyncRead(const PlatformFile& file, size_t offset, void* buffer, size_t size) {
	ASSERT(!m_AioInProgress);

	if (file.m_FileDes == -1) {
		return false;
	}

	if (file.m_Mode != kPlatformFileRead) {
		return false;
	}

	ResetAsync();

	m_AioBlock.aio_fildes = file.m_FileDes;
	m_AioBlock.aio_offset = offset;
	m_AioBlock.aio_buf = buffer;
	m_AioBlock.aio_nbytes = size;
	m_AioBlock.aio_reqprio = 0;
	m_AioBlock.aio_sigevent.sigev_notify = SIGEV_NONE;

	int res = aio_read(&m_AioBlock);

	if (res == -1) {
		return false;
	}

	m_AioInProgress = true;

	return true;
}

bool PlatformFileSystemImpl::AsyncWrite(const PlatformFile& file, size_t offset, void* buffer, size_t size) {
	ASSERT(!m_AioInProgress);

	if (file.m_FileDes == -1) {
		return false;
	}

	if (file.m_Mode != kPlatformFileWrite && file.m_Mode != kPlatformFileAppend) {
		return false;
	}

	ResetAsync();

	m_AioBlock.aio_fildes = file.m_FileDes;
	m_AioBlock.aio_offset = offset;
	m_AioBlock.aio_buf = buffer;
	m_AioBlock.aio_nbytes = size;
	m_AioBlock.aio_reqprio = 0;
	m_AioBlock.aio_sigevent.sigev_notify = SIGEV_NONE;

	int res = aio_write(&m_AioBlock);

	if (res == -1) {
		return false;
	}

	m_AioInProgress = true;

	return true;
}

void PlatformFileSystemImpl::AsyncCancel(const PlatformFile& file) {
	if (m_AioInProgress) {
		aio_cancel(file.m_FileDes, NULL);
	}
}

bool PlatformFileSystemImpl::AsyncComplete(int* status, size_t* size) {
	*status = kAsyncStatusNull;

	if (!m_AioInProgress) {
		return false;
	}

	bool error = false;

	ssize_t res = aio_return(&m_AioBlock);

	// Async either in progress or error
	if (res == -1) {
		int err = aio_error(&m_AioBlock);

		// Returns false if async still in progress
		if (err == EINPROGRESS) {
			*status = kAsyncStatusProgress;
			return false;
		}
		else {
			error = true;
		}
	}

	if (!error) {
		*size = (size_t)res;
		*status = kAsyncStatusSuccess;
	}
	else {
		*status = kAsyncStatusError;
	}

	m_AioInProgress = false;
	ResetAsync();

	// Returns true if the async is successful or error
	return true;
}

bool PlatformFileSystemImpl::AsyncInProgress() const {
	return m_AioInProgress;
}

void PlatformFileSystemImpl::ResetAsync() {
	ASSERT(!m_AioInProgress);

	memset((void*)&m_AioBlock, 0, sizeof(struct aiocb));
}
