#include "GraphicsInfo.h"
#include "BuildInfo.h"

//Platform specific includes
#ifdef PLATFORM_WINDOWS
#include "Windows/Windows.h"
#endif

namespace mge {
#ifdef PLATFORM_WINDOWS
	//Windows implementation
	size_t GetScreenWidth() {
		return WindowsGetScreenWidth();
	}
	size_t GetScreenHeight() {
		return WindowsGetScreenHeight();
	}
	ScreenSize GetScreenSize() {
		return { WindowsGetScreenWidth(), WindowsGetScreenHeight() };
	}
#endif
}