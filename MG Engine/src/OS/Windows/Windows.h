#pragma once
#include "Defines.h"

namespace mge {
	size_t WindowsGetScreenWidth();
	size_t WindowsGetScreenHeight();
	bool8_t WindowsIsWindowActive();

	char_t* WindowsGetTitle();
	void WindowsSetTitle(const char_t* newString);
}
