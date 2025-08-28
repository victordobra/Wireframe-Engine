#include "ComponentType.hpp"
#include <cstring>
#include <stdexcept>

namespace wfe {
	// Component type vector definition
	constinit size_t ComponentType::componentTypeCount = 0;
	constinit ComponentType ComponentType::componentTypes[ComponentType::MAX_COMPONENT_TYPE_COUNT];

	// Public functions
	const ComponentType& ComponentType::GetComponentType(const std::string& name) {
		// Search for the asset type by name
		for(size_t i = 0; i != componentTypeCount; ++i) {
			if(componentTypes[i].name == name)
				return componentTypes[i];
		}

		// If not found, throw an exception
		throw std::invalid_argument("Component type with name \"" + name + "\" not found!");
	}
	void ComponentType::RegisterComponentType(const ComponentType& componentType) {
		// Check if the component type is already registered
		for(size_t i = 0; i != componentTypeCount; ++i) {
			if(!std::strcmp(componentTypes[i].name, componentType.name))
				throw std::invalid_argument((std::string)"Component type with name \"" + componentType.name + "\" is already registered!");
		}

		// Register the new component type
		componentTypes[componentTypeCount++] = componentType;
	}
}