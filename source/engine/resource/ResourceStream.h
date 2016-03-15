#ifndef RESOURCESTREAM_H_
#define RESOURCESTREAM_H_

#include "base_include.h"

#include "allocator/FrameAllocator.h"
#include "platform/PlatformFileSystem.h"

#include "Resource.h"

//--------------------------------------------------
//
// ResourceStream.h
//
// Defines the ResourceStream class and its auxiliary classes and data structs
//
//--------------------------------------------------


// Max data loaded in one io call
const int kResourceLoadSizeMax = MEGABYTES_TO_BYTES(16);

// Status codes for a resource stream operation
const int kResourceStreamNull = 0;
const int kResourceStreamSuccess = 1;
const int kResourceStreamProgress = 2;
const int kResourceStreamError = 3;


enum ResourceBufferStatus_t {
	kResourceBufferEmpty = 0,
	kResourceBufferProgress = 1, // Buffer being used by user or io call
	kResourceBufferWaiting = 2 // Buffer waiting for user to handle
};

//--------------------------------------------------
//
// ResourceBuffer
//
// Stores the data that is read from io operations
//
// Status tracks the current state of the buffer
// ... empty: no data stored - can be used by io call
// ... progress: buffer used by user or io call
// ... waiting: data stored - can be retrieved by user
//
//--------------------------------------------------
struct ResourceBuffer {
	byte_t* data;
	ResourceBufferStatus_t status;
	size_t size; // size of data in the buffer
	bool error; // True if an error occurred

	ResourceUsage_t usage; // For use after loading
};


// Forward declarations
class ResourceStream;

//--------------------------------------------------
//
// ResourceBufferHandle
//
// Handle that contains the pointer to the data after io completion
// Will free the data buffer in resource stream for reuse once deconstructed
//
//--------------------------------------------------
class ResourceBufferHandle {

public:
	// Default constructor creates buffer with a null state
	ResourceBufferHandle();

	ResourceBufferHandle(ResourceStream* stream, byte_t* buffer, size_t size, bool error, ResourceUsage_t usage);

	~ResourceBufferHandle();

	byte_t* GetData() { return m_Buffer; }
	size_t GetSize() const { return m_Size; }

	// Returns true if valid and contains the data
	bool IsValid() const { return m_Buffer != nullptr; }

	bool IsError() const { return m_Error; }

private:
	ResourceStream* m_StreamPtr;
	byte_t* m_Buffer;

	// True if an error occurred during io
	bool m_Error;

	// Size of data in the buffer
	size_t m_Size;

	// Determines how the data will be stored after loading
	ResourceUsage_t m_Usage;
};


// Forward declarations
class ResourceManager;

//--------------------------------------------------
//
// ResourceStream
//
// Reads data from a resource file. Read is only done one at a time
//
// User must actively acquire and release the buffer handle, if not the stream 
// will stall and no other io calls will be done
//
//--------------------------------------------------
class ResourceStream {
	friend class ResourceBufferHandle;

public:
	ResourceStream(ResourceManager* manager, PlatformFileSystem* fileSys);
	~ResourceStream();

	// Called on each frame
	void Update();


	// Uses asynchronous io
	bool LoadDataFromFile(const char* path, ResourceUsage_t usage);


	// Returns a handle to the result of the most recent io operation
	//
	// If there is no data, the handle will be in a null state
	// MUST call ReleaseBufferHandle after the data is handles
	ResourceBufferHandle AcquireBufferHandle();

	// Call after handling the data
	void ReleaseBufferHandle(ResourceBufferHandle& handle);


	// Returns true if the stream is availabe for the next load
	bool CanLoad() const;

	// Returns true when the most recent io operation has finished
	bool IsComplete() const;

private:
	// Checks and handles completion of an io operation
	//
	// Call on each frame
	void CheckForCompletion();

	// Swap the front and back buffer
	//
	// Only call when the front buffer is empty and the back buffer is waiting
	void SwapBuffers();

private:
	// Pointer to the platform file system
	PlatformFileSystem* m_FileSysPtr;
	// Pointer to the resource manager
	ResourceManager* m_ManagerPtr;


	// Buffers used for double-buffering
	ResourceBuffer m_Buffers[2];

	// User can only retrieve data from the front buffer
	ResourceBuffer* m_FrontBuffer;
	// IO can only read data to the back buffer
	ResourceBuffer* m_BackBuffer;

	// File to use in async io
	PlatformFile m_File;
};

#endif