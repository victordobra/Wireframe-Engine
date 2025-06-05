#include "ComponentList.hpp"
#include "EntityManager.hpp"
#include "Core/Memory/Allocator.hpp"
#include <new>

namespace wfe {
	// Internal functions
	ComponentList::ComponentList(EntityManager* entityManager, const ComponentType& componentType) : entityManager(entityManager), componentType(componentType), componentTypeIndex(entityManager->typeIndices.at(componentType.name)), data(AllocMemory(componentType.size * entityManager->maxEntityCount)) {
		// Throw an exception if the allocation failed
		if(!data)
			throw std::bad_alloc();
	}
	ComponentList::~ComponentList() {
		// Destroy all active entities
		for(Entity i = 0; i != entityManager->maxEntityCount; ++i) {
			// Check if the current entity has the current component
			if(entityManager->signatures[i].test(componentTypeIndex))
				componentType.destructor((char*)data + i * componentType.size);
		}

		// Free the list's data
		FreeMemory(data);
	}

	// Public functions
	void* ComponentList::CreateComponent(Entity entity) {
		// Get the component's address
		void* address = (char*)data + entity * componentType.size;

		// Create the component and update the entity's signature, if needed
		if(!entityManager->signatures[entity].test(componentTypeIndex)) {
			componentType.constructor(address);
			entityManager->signatures[entity].set(componentTypeIndex, true);
		}
		
		return address;
	}
	void* ComponentList::GetComponent(Entity entity) const {
		// Check if the component exists
		if(entityManager->signatures[entity].test(componentTypeIndex)) {
			return (char*)data + entity * componentType.size;
		} else {
			return nullptr;
		}
	}
	void ComponentList::DestroyComponent(Entity entity) {
		// Exit the function if the component does not exist
		if(!entityManager->signatures[entity].test(componentTypeIndex))
			return;
		
		// Destroy the component and update the signature
		componentType.destructor((char*)data + entity * componentType.size);
		entityManager->signatures[entity].set(componentTypeIndex, false);
	}
}
