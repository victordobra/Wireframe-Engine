#pragma once

#include "BuildInfo.hpp"

#if defined(PLATFORM_WINDOWS)
#include "WindowsMainWindow.hpp"
#elif defined(PLATFORM_LINUX)
#include "LinuxMainWindow.hpp"
#endif