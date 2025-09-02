#pragma once

#include "Core/Math/General/MatUtils.hpp"
#include "Core/Types/Defines.hpp"
#include "ComponentList.hpp"
#include "ComponentType.hpp"
#include "Transform.hpp"
#include <bitset>
#include <string>
#include <unordered_map>

namespace wfe {
	class Program;

	/// @brief A class managing all entities, their components and the overarching systems.
	class EntityManager {
	public:
		/// @brief The handle of an invalid entity.
		static inline const Entity INVALID_ENTITY = SIZE_T_MAX; 

		/// @brief An entity's signature, checking what components it has.
		typedef std::bitset<ComponentType::MAX_COMPONENT_TYPE_COUNT> Signature;

		/// @brief Creates an entity manager.
		/// @param maxEntityCount The maximum number of entities.
		EntityManager(size_t maxEntityCount);
		EntityManager(const EntityManager&) = delete;
		EntityManager(EntityManager&&) = delete;

		EntityManager& operator=(const EntityManager&) = delete;
		EntityManager& operator=(EntityManager&&) = delete;

		/// @brief Gets the maximum number of entities managed.
		/// @return The maximum number of entities managed.
		size_t GetMaxEntityCount() const {
			return maxEntityCount;
		}
		/// @brief Gets the given entity's transform.
		/// @param entity The entity whose transform to get.
		/// @return A reference to the entity's transform.
		Transform& GetEntityTransform(Entity entity) {
			return transforms[entity];
		}
		/// @brief Gets the given entity's transform.
		/// @param entity The entity whose transform to get.
		/// @return A const reference to the entity's transform.
		const Transform& GetEntityTransform(Entity entity) const {
			return transforms[entity];
		}
		/// @brief Gets the given entity's signature.
		/// @param entity The entity whose signature to get.
		/// @return The entity's signature.
		const Signature& GetEntitySignature(Entity entity) const {
			return signatures[entity];
		}

		/// @brief Gets the parent of the given entity.
		/// @param entity The entity whose parent to get.
		/// @return The parent of the given entity, or INVALID_ENTITY if the entity has no parent.
		Entity GetParent(Entity entity) const {
			return parents[entity];
		}
		/// @brief Gets the children of the given entity.
		/// @param entity The entity whose children to get.
		/// @return A vector containing the children of the given entity.
		const std::vector<Entity>& GetChildren(Entity entity) const {
			return children[entity];
		}
		/// @brief Sets the parent of the given entity.
		/// @param entity The entity whose parent to set.
		/// @param newParent The new parent of the entity, or INVALID_ENTITY if the entity should not have a parent.
		void SetParent(Entity entity, Entity newParent) {
			// Exit the function if the parent isn't changed
			if(parents[entity] == newParent)
				return;

			// Remove the entity from the old children vector
			RemoveEntityFromParent(entity);

			// Set the entity's parent and and it to the children vector
			parents[entity] = newParent;
			if(newParent != EntityManager::INVALID_ENTITY)
				children[newParent].push_back(entity);
		}

		/// @brief Updates the global transform maitrices for every entity.
		void UpdateGlobalTransforms();
		/// @brief Gets the global transform matrix of the given entity at the time the global transforms were last updated.
		/// @param entity The entity whose transform to get.
		/// @return The 4x4 single-precision floating point matrix describing the global transform of the entity.
		const Mat4x4f& GetGlobalTransform(Entity entity) const {
			return globalTransforms[entity];
		}
		/// @brief Gets the global inverse transform matrix of the given entity at the time the global transforms were last updated.
		/// @param entity The entity whose inverse transform to get.
		/// @return The 4x4 single-precision floating point matrix describing the global inverse transform of the entity.
		const Mat4x4f& GetGlobalInvTransform(Entity entity) const {
			return globalInvTransforms[entity];
		}
		/// @brief Gets the global normal transform matrix of the given entity at the time the global transforms were last updated.
		/// @param entity The entity whose normal transform to get.
		/// @return The 4x4 single-precision floating point matrix describing the global normal transform of the entity.
		const Mat4x4f& GetGlobalNormalTransform(Entity entity) const {
			return globalNormalTransforms[entity];
		}

		/// @brief Gets the given type's component list.
		/// @param typeIndex The component list type's index.
		/// @return The given type's component list.
		ComponentList* GetComponentList(size_t typeIndex) const {
			return componentLists[typeIndex];
		}
		/// @brief Gets the index of the requested component type.
		/// @param name The name of the component type.
		/// @return The index of the requested component type.
		size_t GetTypeIndex(const std::string& name) const {
			return typeIndices.at(name);
		}
		/// @brief Gets the index of the requested component type.
		/// @tparam T The component type.
		/// @return The index of the requested component type.
		template<class T>
		size_t GetTypeIndex() const {
			return GetTypeIndex(typeid(T).name());
		}

		/// @brief Creates an entity.
		/// @return The entity's handle, or INVALID_ENTITY if the maximum entity count was reached.
		Entity CreateEntity();
		/// @brief Destroys an entity.
		/// @return The handle of the entity to destroy.
		void DestroyEntity(Entity entity);
		/// @brief Gets the next entity containing any of the requested components.
		/// @param prev The previous entity in the order of traversal, or INVALID_ENTITY if the first entity with any of the requested component types should be returned.
		/// @param available A signature containing all componeny types accepted by the traversal.
		/// @return The handle of the next entity containing any of the requested components.
		Entity GetNextEntity(Entity prev, Signature available);

		/// @brief Destroys the entity manager.
		~EntityManager();
	private:
		friend ComponentList;

		void RemoveEntityFromParent(Entity entity);
		void UpdateEntityGlobalTransform(Entity entity, const Mat4x4f& parentTransform, const Mat4x4f& parentInvTransform);

		size_t maxEntityCount;
		Entity firstFree;
		Entity* freeList;
		std::bitset<ComponentType::MAX_COMPONENT_TYPE_COUNT>* signatures;

		Entity* parents;
		std::vector<Entity>* children;

		Mat4x4f* globalTransforms;
		Mat4x4f* globalInvTransforms;
		Mat4x4f* globalNormalTransforms;

		Transform* transforms;
		std::vector<ComponentList*> componentLists;

		std::unordered_map<std::string, size_t> typeIndices;
	};
}