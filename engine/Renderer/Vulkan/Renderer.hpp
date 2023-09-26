#pragma once

#include "Core.hpp"
#include "Allocator.hpp"
#include "Instance.hpp"

namespace wfe {
	/// @brief Creates a Vulkan API rendering backend.
	/// @return True if Vulkan is supported and the backend was created successfully, otherwise false.
	bool8_t CreateVulkanRenderer();
	/// @brief Deletes the Vulkan API rendering backend.
	void DeleteVulkanRenderer();
}