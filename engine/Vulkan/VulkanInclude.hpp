#pragma once
#include "Core.hpp"

#ifdef PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdefaulted-function-deleted"
#include "vulkan.hpp"
#pragma clang diagnostic pop

#define WFE_VULKAN_UTILS_DONT_INCLUDE_VULKAN
#include "VulkanUtils.hpp"