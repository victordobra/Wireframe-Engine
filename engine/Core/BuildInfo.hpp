#pragma once

#include <stddef.h>

// Windows
#if defined(WIN32) | defined(_WIN32) | defined(__WIN32__)
/// @brief Defined when building for Windows.
#define WFE_PLATFORM_WINDOWS
#endif

// Linux
#if defined(__linux__) || defined(__gnu_linux__)
/// @brief Defined when building for Linux.
#define WFE_PLATFORM_LINUX
#endif

// Check if the target platform was detected
#if !defined(WFE_PLATFORM_WINDOWS) && !defined(WFE_PLATFORM_LINUX)
#error "Failed to find the target platform!"
#endif

// Architecture
#if defined(__i386__)
// Defined when building for a 32-bit architecture
#define WFE_ARCHITECTURE_I386
#else
// Defined when building for a 64-bit architecture
#define WFE_ARCHITECTURE_X86_64
#endif

// Build mode
#if !defined(NDEBUG)
// Defined when building in debug mode
#define WFE_BUILD_MODE_DEBUG
#else
// Defined when building in release mode
#define WFE_BUILD_MODE_RELEASE
#endif