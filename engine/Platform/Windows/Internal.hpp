#pragma once

#include <Core.hpp>
#include "Platform/KeyCode.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef CreateWindow

namespace wfe {
	/// @brief Converts the given virtual key code to an engine key code.
	KeyCode VKToKeyCode(WPARAM wParam);
	/// @brief Passes the event with the given parameters to the input system.
	LRESULT CALLBACK InputWinProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
}