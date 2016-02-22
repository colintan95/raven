#ifndef RESOURCEHANDLE_H_
#define RESOURCEHANDLE_H_

#include "base_include.h"

// Forward declarations
class Resource;

//--------------------------------------------------
//
// ResourceHandle
//
// Interface to access data stored in a resource
//
// Handle increments the resource usage counter when created, and decrements
// the counter when it is destroyed
//
//--------------------------------------------------
class ResourceHandle {
	friend class Resource;

private:
	// Handle can only be instantiated by a resource
	ResourceHandle(Resource* resource);

public:
	ResourceHandle();
	ResourceHandle(const ResourceHandle& handle);
	~ResourceHandle();

	const byte_t* GetData() const { return m_DataPtr; }

	// Returns true if handle points to a valid resource
	bool IsValid() const { return (m_ResourcePtr != nullptr); }

private:
	// Pointer to the resource
	Resource* m_ResourcePtr;

	// Pointer to the resource data
	const byte_t* m_DataPtr;
};

#endif