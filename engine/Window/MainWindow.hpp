#pragma once
#include "BuildInfo.hpp"

#if defined(PLATFORM_WINDOWS)
#include "Windows/MainWindow.hpp"
#elif defined(PLATFORM_LINUX)
#include "Linux/MainWindow.hpp"
#endif
