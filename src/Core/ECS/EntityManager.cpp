#include "EntityManager.hpp"
#include "Core/Memory/Allocator.hpp"
#include <new>

namespace wfe {
	// Public functions
	EntityManager::EntityManager(size_t maxEntityCount) : maxEntityCount(maxEntityCount), firstFree(0), freeList(new Entity[maxEntityCount]), signatures(new Signature[maxEntityCount]) {
		// Set the free list values
		for(Entity i = 0; i != maxEntityCount - 1; ++i)
			freeList[i] = i + 1;
		freeList[maxEntityCount - 1] = INVALID_ENTITY;

		// Set the component type indices
		for(size_t i = 0; i != ComponentType::GetComponentTypeCount(); ++i)
			typeIndices.insert({ (std::string)ComponentType::GetComponentTypes()[i].name, i });

		// Create the component lists
		componentLists.resize(ComponentType::GetComponentTypeCount());
		for(size_t i = 0; i != componentLists.size(); ++i)
			componentLists[i] = new ComponentList(this, ComponentType::GetComponentTypes()[i]);
	}

	Entity EntityManager::CreateEntity() {
		// Check if the free list is empty
		if(firstFree == INVALID_ENTITY)
			return INVALID_ENTITY;
		
		// Get the entity from the free list
		Entity entity = firstFree;
		firstFree = freeList[entity];

		return entity;
	}
	void EntityManager::DestroyEntity(Entity entity) {
		// Destroy the entity's components
		for(size_t i = 0; i != componentLists.size(); ++i)
			componentLists[i]->DestroyComponent(entity);

		// Add the entity to the free list
		freeList[entity] = firstFree;
		firstFree = entity;
	}
	Entity EntityManager::GetNextEntity(Entity prev, Signature available) {
		// Loop through all entities, starting from the given index
		for(Entity i = prev + 1; i != maxEntityCount; ++i) {
			// Check if the current index has at least one of the requested components
			if((signatures[i] & available).any())
				return i;
		}

		// No entity was found; return an invalid handle
		return INVALID_ENTITY;
	}

	EntityManager::~EntityManager() {
		// Destroy the component lists
		for(size_t i = 0; i != ComponentType::GetComponentTypeCount(); ++i)
			delete componentLists[i];
		
		// Destroy the signature and free lists
		delete[] freeList;
		delete[] signatures;
	}
}