#pragma once

#include "Core/Types/Defines.hpp"
#include "ComponentList.hpp"
#include "ComponentType.hpp"
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
		/// @brief Gets the given entity's signature.
		/// @param entity The entity whose signature to get.
		/// @return The entity's signature.
		const Signature& GetEntitySignature(Entity entity) const {
			return signatures[entity];
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

		size_t maxEntityCount;
		Entity firstFree;
		Entity* freeList;
		std::bitset<ComponentType::MAX_COMPONENT_TYPE_COUNT>* signatures;
		std::vector<ComponentList*> componentLists;

		std::unordered_map<std::string, size_t> typeIndices;
	};
}