#pragma once

#include "Core/Types/Defines.hpp"
#include <string>
#include <typeindex>

namespace wfe {
	/// @brief The entity handle type.
	typedef size_t Entity;

	/// @brief A structure that defines a component type.
    struct ComponentType {
    public:
		/// @brief The maximum possible total number of component types
		static inline const size_t MAX_COMPONENT_TYPE_COUNT = 128;

		/// @brief The component constructor function type.
		typedef void(*Constructor)(void* address);
		/// @brief The component destructor function type.
        typedef void(*Destructor)(void* address);

		/// @brief Gets the number of registered component types.
		/// @return The number of registered component types.
		static size_t GetComponentTypeCount() {
			return componentTypeCount;
		}
		/// @brief Gets all registered component types.
		/// @return A pointer to the array of all registered component types.
		static const ComponentType* GetComponentTypes() {
			return componentTypes;
		}
		/// @brief Gets the component type with the given name.
		/// @param name The component type's name.
		/// @return A reference to the component type with the given name.
		static const ComponentType& GetComponentType(const std::string& name);
		/// @brief Registers a new component type.
		/// @param assetType The component type to register.
		static void RegisterComponentType(const ComponentType& componentType);

		/// @brief Creates a default component type.
		constexpr ComponentType() = default;
		/// @brief Copies the given component type.
		/// @param other The component type to copy.
		constexpr ComponentType(const ComponentType& other) = default;
		/// @brief Moves the given component type.
		/// @param other The component type to move.
		constexpr ComponentType(ComponentType&& other) noexcept = default;

		/// @brief Copies the given component type's info into this component type.
		/// @param other The component type whose info to copy.
		/// @return A reference to this component type.
		ComponentType& operator=(const ComponentType& other) = default;
		/// @brief Moves the given component type's info into this component type.
		/// @param other The component type whose info to move.
		/// @return A reference to this component type.
		ComponentType& operator=(ComponentType&& other) noexcept = default;

		/// @brief Destroys the component type.
		~ComponentType() = default;

		/// @brief The type's name, used for identification.
		const char* name = "";
		/// @brief The size, in bytes, of a component instance.
		size_t size = 0;
		/// @brief The constructor function for the type.
		Constructor constructor = nullptr;
		/// @brief The destructor function for the type.
		Destructor destructor = nullptr;
    private:
		static size_t componentTypeCount;
		static ComponentType componentTypes[MAX_COMPONENT_TYPE_COUNT];
    };

/// @brief A macro to register a component type.
/// @param type The component type to register.
#define WFE_COMPONENT_TYPE(type) \
struct ComponentType##type##Constructor { \
	static void CreateComponent(void* address) { \
		new(address) type; \
	} \
	static void DestroyComponent(void* address) { \
		((type*)address)->~type(); \
	} \
	ComponentType##type##Constructor() { \
		wfe::ComponentType componentType; \
		\
		componentType.name = typeid(type).name(); \
		componentType.size = sizeof(type); \
		componentType.constructor = CreateComponent; \
		componentType.destructor = DestroyComponent; \
		\
		wfe::ComponentType::RegisterComponentType(componentType); \
	} \
}; \
static inline ComponentType##type##Constructor componentType##type##ConstructorInstance { };
}