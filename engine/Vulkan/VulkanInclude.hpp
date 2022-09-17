#pragma once
#include "Core.hpp"

#if defined(PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(PLATFORM_LINUX)
#define VK_USE_PLATFORM_XLIB_KHR
#endif

#include "vulkan.h"

#define WFE_VULKAN_UTILS_DONT_INCLUDE_VULKAN
#include "VulkanUtils.hpp"