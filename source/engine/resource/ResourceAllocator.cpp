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

byte_t* ResourceAllocator::Allocate(size_t size, ResourceUsage_t usage) {
	if (usage == kResourceSingleUse) {
		
	}
}