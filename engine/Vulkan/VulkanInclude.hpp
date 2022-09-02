#pragma once
#include "Core.hpp"

#if defined(PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(PLATFORM_LINUX)
#define VK_USE_PLATFORM_XCB_KHR
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdefaulted-function-deleted"
#include "vulkan.h"
#pragma clang diagnostic pop

#define WFE_VULKAN_UTILS_DONT_INCLUDE_VULKAN
#include "VulkanUtils.hpp"