#include "EntityManager.hpp"
#include "Core/Memory/Allocator.hpp"
#include <new>

namespace wfe {
	// Internal helper functions
	void EntityManager::RemoveEntityFromParent(Entity entity) {
		// Exit the function if the entity has no parent
		Entity parent = parents[entity];
		if(parent == INVALID_ENTITY)
			return;
		
		// Find the entity in the parent's vector
		for(size_t i = 0; i != children[parent].size(); ++i) {
			if(children[parent][i] == entity) {
				children[parent].erase(children[parent].begin() + i);
				break;
			}
		}
	}
	void EntityManager::UpdateEntityGlobalTransform(Entity entity, const Mat4x4f& parentTransform, const Mat4x4f& parentInvTransform) {
		// Calculate the local space transforms
		Transform transform = transforms[entity];

		Mat4x4f localTransform = Mat4x4Translate(transform.pos) * Mat4x4Rotate(transform.rot) * Mat4x4Scale(transform.scale);
		Mat4x4f localInvTransform = Mat4x4Scale(1.0f / transform.scale) * Mat4x4Rotate(QuatConjugate(transform.rot)) * Mat4x4Translate(-transform.pos);

		// Update the entity's transforms based on the given matrices
		globalTransforms[entity] = parentTransform * localTransform;
		globalInvTransforms[entity] = localInvTransform * parentInvTransform;

		// Update the transforms of all children
		for(size_t i = 0; i != children[entity].size(); ++i)
			UpdateEntityGlobalTransform(children[entity][i], globalTransforms[entity], globalInvTransforms[entity]);
	}

	// Public functions
	EntityManager::EntityManager(size_t maxEntityCount) : maxEntityCount(maxEntityCount), firstFree(0), freeList(new Entity[maxEntityCount]), signatures(new Signature[maxEntityCount]), parents(new Entity[maxEntityCount]), children(new std::vector<Entity>[maxEntityCount]), globalTransforms(new Mat4x4f[maxEntityCount]), globalInvTransforms(new Mat4x4f[maxEntityCount]), globalNormalTransforms(new Mat4x4f[maxEntityCount]), transforms(new Transform[maxEntityCount]) {
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

	void EntityManager::UpdateGlobalTransforms() {
		// Calculate all basic global transforms
		for(size_t i = 0; i != maxEntityCount; ++i) {
			// Skip the current entity if it not a root entity
			if(parents[i] != INVALID_ENTITY)
				continue;
			
			// Update the entity's transforms using the identity matrix as parent transforms
			UpdateEntityGlobalTransform(i, MAT4X4F_IDENTITY, MAT4X4F_IDENTITY);
		}

		// Calculate the global normal transforms
		for(size_t i = 0; i != maxEntityCount; ++i) {
			// Skip the current entity if it does not exist
			if(parents[i] == maxEntityCount)
				continue;
			
			// Calculate the entity's normal transform
			globalNormalTransforms[i] = MatTranspose(globalInvTransforms[i]);

			// Remove the normal transform's translate component
			globalNormalTransforms[i][3][0] = 0.0f;
			globalNormalTransforms[i][3][1] = 0.0f;
			globalNormalTransforms[i][3][2] = 0.0f;
		}
	}

	Entity EntityManager::CreateEntity() {
		// Check if the free list is empty
		if(firstFree == INVALID_ENTITY)
			return INVALID_ENTITY;
		
		// Get the entity from the free list
		Entity entity = firstFree;
		firstFree = freeList[entity];

		// Set the entity's parent
		parents[entity] = INVALID_ENTITY;

		// Reset the entity's transform
		transforms[entity] = {
			.pos = VEC3F_ZERO,
			.rot = QUATF_IDENTITY,
			.scale = VEC3F_ONE
		};

		return entity;
	}
	void EntityManager::DestroyEntity(Entity entity) {
		// Destroy the entity's components
		for(size_t i = 0; i != componentLists.size(); ++i)
			componentLists[i]->DestroyComponent(entity);
		
		// Reset the entity's signature
		signatures[entity].reset();
		
		// Clear the entity's children vector and remove it from its parent's child vector
		children[entity].clear();
		RemoveEntityFromParent(entity);
		
		// Set the entity's parent to an invalid value, so it can be recognised as nonexistent
		parents[entity] = maxEntityCount;

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
		
		// Destroy all created arrays
		delete[] freeList;
		delete[] signatures;
		delete[] parents;
		delete[] children;
		delete[] globalTransforms;
		delete[] globalInvTransforms;
		delete[] globalNormalTransforms;
		delete[] transforms;
	}
}