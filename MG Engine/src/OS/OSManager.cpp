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
	static size_t screenWidth = WindowsGetScreenWidth();
	static size_t screenHeight = WindowsGetScreenHeight();
	static size_t gameWidth = WindowsGetGameWidth();
	static size_t gameHeight = WindowsGetGameHeight();

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
		return screenWidth;
	}
	size_t OSMGetScreenHeight() {
		return screenHeight;
	}
	size_t OSMGetGameWidth() {
		return gameWidth;
	}
	size_t OSMGetGameHeight() {
		return gameHeight;
	}
}