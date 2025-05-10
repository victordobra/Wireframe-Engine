#pragma once

#include "Core/Types/Defines.hpp"
#include <source_location>
#include <string>

namespace wfe {
	class DummyType;

	/// @brief Gets the function path of this function. Used internally to get the type name.
	/// @tparam T The template type.
	/// @return The function path of this function.
	template<class T>
	inline std::string GetPath() {
		return std::source_location::current().function_name();
	}

	/// @brief Gets the type name of the specified type.
	/// @tparam T The type to get the name of.
	/// @return The name of the type.
	template<class T>
	inline std::string GetTypeName() {
		// Get the function path for a dummy type and the dummy type's position
		std::string dummyPath = GetPath<DummyType>();
		size_t typePos = dummyPath.rfind("DummyType");
		size_t suffixSize = dummyPath.size() - typePos - sizeof("DummyType") + 1;

		// Get the function path for the current type and get the equivalent type
		std::string typePath = GetPath<T>();
		size_t typeLength = typePath.size() - typePos - suffixSize;

		return typePath.substr(typePos, typeLength);
	}

/// @brief Macro to get the type name of a type or variable.
/// @param type The type or variable to get the type name of.
#define WFE_TYPE_NAME(type) GetTypeName<decltype(type)>()
};