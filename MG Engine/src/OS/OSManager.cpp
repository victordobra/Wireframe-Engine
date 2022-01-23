#include "OSManager.h"
#include "OSManagerInternal.h"

#include "GameLoopManagerInternal.h"

#pragma region OS Specific Includes

#ifdef _WIN32
#include "Windows\Windows.h"
#endif

#pragma endregion

namespace mge {
#ifdef _WIN32
	const char* OSMGetTitle() {
		return WindowsGetTitle();
	}
	void OSMSetTitle(const char* newTitle) {
		WindowsSetTitle(newTitle);
	}
	void OSMSetGameSize(size_t newWidth, size_t newHeight) {
		WindowsSetGameSize(newWidth, newHeight);
	}
#endif

	void OSMOnStart() {
		GLMStart();
	}
	void OSMOnUpdate() {
		GLMUpdate();
	}
	void OSMOnButtonDown(int keyCode) {
		GLMOnKeyDown(keyCode);
	}
	void OSMOnButtonUp(int keyCode) {
		GLMOnKeyUp(keyCode);
	}
	void OSMSetMousePos(size_t newX, size_t newY) {
		GLMSetMousePos(newX, newY);
	}

	size_t OSMGetScreenWidth() {
		return WindowsGetScreenWidth();
	}
	size_t OSMGetScreenHeight() {
		return WindowsGetScreenHeight();
	}
	size_t OSMGetGameWidth() {
		return WindowsGetGameWidth();
	}
	size_t OSMGetGameHeight() {
		return WindowsGetGameHeight();
	}
}