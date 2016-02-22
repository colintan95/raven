#ifndef RESOURCEALLOCATOR_H_
#define RESOURCEALLOCATOR_H_

#include "base_include.h"

#include "allocator/FrameAllocator.h"

#include "Resource.h"

// Forward declarations
class ResourceManager;

// Uses 256mb in total:
// ... SingleUse: 32mb
// ... Contextual: 160mb
// ... Permanent: 64mb
const size_t kResourceAllocSingleUseMax = MEGABYTES_TO_BYTES(32);
const size_t kResourceAllocContextMax = MEGABYTES_TO_BYTES(160);
const size_t kResourceAllocPermMax = MEGABYTES_TO_BYTES(64);

//--------------------------------------------------
//
// ResourceAllocator
//
// Allocates memory for all resources in the game
//
// Allocation strategy differs for resources with different usage
//
//--------------------------------------------------
class ResourceAllocator {

public:
	ResourceAllocator(ResourceManager* manager);
	~ResourceAllocator();

	// Allocate a region of memory for the resource
	//
	// Type of allocator used depends on the usage specified
	byte_t* Allocate(size_t size, ResourceUsage_t usage);

private:
	// Pointer to the resource manager
	ResourceManager* m_ManagerPtr;

	// Allocator for single use resources
	FrameAllocator m_SingleUseAlloc;

	// Allocator for permanent resources
	FrameAllocator m_PermAlloc;
};

#endif