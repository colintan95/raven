#include "ResourceAllocator.h"

#include "ResourceManager.h"

ResourceAllocator::ResourceAllocator(ResourceManager* manager): 
m_SingleUseAlloc(kResourceAllocSingleUseMax),
m_PermAlloc(kResourceAllocPermMax) {

	m_ManagerPtr = manager;
}

ResourceAllocator::~ResourceAllocator() {
	m_ManagerPtr = nullptr;
}