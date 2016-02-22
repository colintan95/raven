#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "base_include.h"

typedef uint64_t ResourceId_t;

// How the resource is used by the client; determines how the resource is 
// stored
enum ResourceUsage_t {
	kResourceSingleUse, // Used only once then released
	kResourceContextual, // Used for a certain period of the game
	kResourcePermanent // Used for the entire game
};

// Forward declarations
class ResourceHandle;

//--------------------------------------------------
//
// Resource
//
// Contains the data for a resource
//
//--------------------------------------------------
class Resource {
	friend class ResourceHandle;

public:
	Resource();
	~Resource();

	ResourceHandle CreateHandle();

	const byte_t* GetData() const { return m_Data; }

	// True if resource is still referenced via its handle
	bool IsUsed() const { return (m_HandleCounter > 0); }

private:
	// Used by a resource handle to modify the handle counter
	void IncrementHandleCounter();
	void DecrementHandleCounter();

private:
	// Pointer to the data
	byte_t* m_Data;

	// Counts the number of handles being used
	int m_HandleCounter;
};

#endif