#pragma once
#include "BuildInfo.hpp"

#pragma warning( push )
#pragma warning( disable : 26812 )
#pragma warning( disable : 26495 )
#pragma warning( disable : 26451 )
#pragma warning( disable : 28251 )
#ifdef PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "vulkan.hpp"
#pragma warning( pop )