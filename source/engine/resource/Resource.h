#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "base_include.h"

typedef uint64_t ResourceId_t;

// Type of resource being loaded; determines post-load processing if needed
enum ResourceType_t {
	kResourceTypeData, // Generic data that does not need post-load
	kResourceTypePng,
	kResourceTypeJpeg
};

// Forward declarations
class ResourceHandle;
class ResourceManager;

//--------------------------------------------------
//
// Resource
//
// Contains the data for a resource
//
//--------------------------------------------------
class Resource {
	friend class ResourceHandle;
	friend class ResourceManager;

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
	//
	// Data can point to nullptr though resource is valid; e.g. png resource
	byte_t* m_Data;

	// Counts the number of handles being used
	int m_HandleCounter;
};

#endif