#pragma once

#include <Core.hpp>

namespace wfe {
	/// @brief All possible rendering backend types.
	typedef enum {
		/// @brief The rendering backend uses the Vulkan API.
		RENDERER_BACKEND_VULKAN,
		/// @brief The number of backend rendering API implementations.
		RENDERER_BACKEND_COUNT
	} RendererBackendType;

	/// @brief Creates the rendering backend.
	void CreateRenderer();
	/// @brief Deletes the rendering backend.
	void DeleteRenderer();

	/// @brief Gets the active renderer backend's type.
	/// @return The active renderer backend's type.
	RendererBackendType GetRendererBackendType();
}