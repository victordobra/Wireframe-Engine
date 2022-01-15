#pragma once

namespace mge {
	size_t WindowsGetScreenWidth();
	size_t WindowsGetScreenHeight();
	size_t WindowsGetGameWidth();
	size_t WindowsGetGameHeight();
	void WindowsSetGameSize(size_t newWidth, size_t newHeight);
	char* WindowsGetTitle();
	void WindowsSetTitle(const char* newString);
}
