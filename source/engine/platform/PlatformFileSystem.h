#ifndef PLATFORMFILESYSTEM_H_
#define PLATFORMFILESYSTEM_H_

#include "base_include.h"

// Include the PlatformFile implementation
#if defined(_PLATFORM_WIN) 

#elif defined(_PLATFORM_OSX)
	#include "osx/PlatformFile_osx.h"
#elif defined(_PLATFORM_LINUX)

#endif

const int kAsyncStatusNull = 0;
const int kAsyncStatusSuccess = 1;
const int kAsyncStatusProgress = 2;
const int kAsyncStatusError = 3;

// Forward declarations
class PlatformFileSystemImpl;

//--------------------------------------------------
//
// PlatformFileSystem
//
// Interface to use the platform's file system
// Only ONE async io operation can be done at one time
//
// Implementation of class in the specific platform's folder
//
//--------------------------------------------------
class PlatformFileSystem {

public:
	PlatformFileSystem();
	~PlatformFileSystem();

	// Reads data from the file asynchronously
	//
	// Parameters:
	// ... file: file to read or write
	// ... offset: offset from the start of the file
	// ... buffer: memory to read to or write from
	// ... size: maximum number of bytes to read or write
	//
	bool AsyncRead(const PlatformFile& file, size_t offset, void* buffer, size_t size);

	// Writes data to the file asynchronously
	//
	// Parameters:
	// ... file: file to read or write
	// ... offset: offset from the start of the file
	// ... buffer: memory to read to or write from
	// ... size: maximum number of bytes to read or write
	//
	bool AsyncWrite(const PlatformFile& file, size_t offset, void* buffer, size_t size);

	// Tries to cancel the async io operation on file
	//
	// If cancelled, AsyncComplete() will set the status as error
	void AsyncCancel(const PlatformFile& file);

	// Returns true if IO is complete
	//
	// Status will be set to success if io was succcessful, if not will be set 
	// to an error code
	// If success, paramater size will be set to the number of bytes read or 
	// written
	bool AsyncComplete(int* status, size_t* size);

	// Returns true if an async io has been initiated and until 
	// AsyncComplete() has been called
	bool AsyncInProgress() const;

private:
	// Platform specific implementation
	PlatformFileSystemImpl* m_Impl;
};

#endif