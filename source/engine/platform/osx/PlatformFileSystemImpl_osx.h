#ifndef PLATFORMFILESYSTEMIMPL_OSX_H_
#define PLATFORMFILESYSTEMIMPL_OSX_H_

#include "base_include.h"

#include <aio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

// Forward declarations
class PlatformFile;

//--------------------------------------------------
//
// PlatformFileSystemImpl
//
// OSX Implementation of the platform file system 
//
//--------------------------------------------------
class PlatformFileSystemImpl {

public:
	PlatformFileSystemImpl();
	~PlatformFileSystemImpl();

	bool AsyncRead(const PlatformFile& file, size_t offset, void* buffer, size_t size);
	bool AsyncWrite(const PlatformFile& file, size_t offset, void* buffer, size_t size);
	void AsyncCancel(const PlatformFile& file);

	bool AsyncComplete(int* status, size_t* size);

	bool AsyncInProgress() const;

private:

	// Resets all async states and prepares for the next operation
	//
	// Call only if m_AioInProgress is false
	void ResetAsync();

private:
	struct aiocb m_AioBlock;

	// True if an async io is ongoing
	//
	// Only set to false when AsyncComplete is called
	bool m_AioInProgress;
};


#endif