#pragma once

#include "Core/Types/Defines.hpp"
#include "ComponentType.hpp"

namespace wfe {
	class EntityManager;

	/// @brief A class implementing an array of fcomponents of a specific type.
	class ComponentList {
	public:
		ComponentList() = delete;
		ComponentList(const ComponentList&) = delete;
		ComponentList(ComponentList&&) = delete;

		ComponentList& operator=(const ComponentList&) = delete;
		ComponentList& operator=(ComponentList&&) = delete;

		/// @brief Gets the entity manager owning the component list.
		/// @return The entity manager owning the component list.
		EntityManager* GetEntityManager() const {
			return entityManager;
		}
		/// @brief Gets the type of the components managed by the list.
		/// @return The type of the components managed by the list.
		const ComponentType& GetComponentType() const {
			return componentType;
		}
		/// @brief Gets the index of the component type in the entity manager.
		/// @return The index of the component type in the entity manager.
		size_t GetComponentTypeIndex() const {
			return componentTypeIndex;
		}

		/// @brief Creates a new component for the current entity.
		/// @param entity The entity to create a new component for.
		/// @return A pointer to the newly created component. If the component already existed, a pointer to the old component instance is returned.
		void* CreateComponent(Entity entity);
		/// @brief Gets the component for the given entity.
		/// @param entity The entity to get the component from.
		/// @return A pointer to the entity's component, or nullptr if it does not exist.
		void* GetComponent(Entity entity) const;
		/// @brief Destroys the component of the given entity.
		/// @param entity The entity whose component to destroy.
		void DestroyComponent(Entity entity);
	private:
		friend EntityManager;

		ComponentList(EntityManager* entityManager, const ComponentType& componentType);
		~ComponentList();

		EntityManager* entityManager;
		ComponentType componentType;
		size_t componentTypeIndex;
		void* data;
	};
}