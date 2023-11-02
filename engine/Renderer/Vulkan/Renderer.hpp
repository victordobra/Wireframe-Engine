#pragma once

#include <Core.hpp>
#include "Loader/Loader.hpp"
#include "Core/Allocator.hpp"
#include "Core/CommandPool.hpp"
#include "Core/Device.hpp"
#include "Core/Instance.hpp"
#include "Core/Surface.hpp"
#include "Core/SwapChain.hpp"

#include <vulkan/vulkan.h>

namespace wfe {
	/// @brief Creates a Vulkan API rendering backend.
	/// @return True if Vulkan is supported and the backend was created successfully, otherwise false.
	bool8_t CreateVulkanRenderer();
	/// @brief Destroys the Vulkan API rendering backend.
	void DestroyVulkanRenderer();
}