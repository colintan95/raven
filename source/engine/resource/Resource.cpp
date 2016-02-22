#include "Resource.h"

#include "ResourceHandle.h"

Resource::Resource() {
	m_HandleCounter = 0;

    m_Data = nullptr;
}

Resource::~Resource() {
	m_HandleCounter = 0;
	
	m_Data = nullptr;
}

ResourceHandle Resource::CreateHandle() {
	return ResourceHandle(this);
}

void Resource::IncrementHandleCounter() {
	++m_HandleCounter;
}

void Resource::DecrementHandleCounter() {
	ASSERT(m_HandleCounter > 0);

	if (m_HandleCounter > 0) {
		--m_HandleCounter;
	}
}