#pragma once
#include "Defines.h"
#include "VulkanInclude.h"

namespace mge {
	struct ScreenSize {
		size_t width;
		size_t height;
	};

	size_t GetScreenWidth();
	size_t GetScreenHeight();
}