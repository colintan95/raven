#include "ResourceHandle.h"

#include "Resource.h"

ResourceHandle::ResourceHandle(Resource* resource) {
	ASSERT(resource != nullptr);

	m_ResourcePtr = resource;
	m_DataPtr = m_ResourcePtr->GetData();

	m_ResourcePtr->IncrementHandleCounter();
}

ResourceHandle::ResourceHandle() {
	m_ResourcePtr = nullptr;
	m_DataPtr = nullptr;
}

ResourceHandle::ResourceHandle(const ResourceHandle& handle) {
	m_ResourcePtr = handle.m_ResourcePtr;
	m_DataPtr = handle.m_DataPtr;

	if (m_ResourcePtr != nullptr) {
		m_ResourcePtr->IncrementHandleCounter();
	}
}

ResourceHandle::~ResourceHandle() {
	if (m_ResourcePtr != nullptr) {
		m_ResourcePtr->DecrementHandleCounter();
	}

	m_DataPtr = nullptr;
	m_ResourcePtr = nullptr;
}

