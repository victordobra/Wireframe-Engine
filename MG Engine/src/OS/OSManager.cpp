#include "OSManager.h"
#include "OSManagerInternal.h"

#include "BuildData.h"
#include "GameLoopManagerInternal.h"

#pragma region OS Specific Includes

#if BUILD_PLATFORM_WINDOWS
#include "Windows\Windows.h"
#endif

#pragma endregion

#pragma region Variables

static unsigned int& ScreenWidth = Window::GetScreenWidthRef();
static unsigned int& ScreenHeight = Window::GetScreenHeightRef();
static unsigned int& GameWidth = Window::GetGameWidthRef();
static unsigned int& GameHeight = Window::GetGameHeightRef();
unsigned long*& OSMScreenColours = Window::GetScreenColoursRef();

#pragma endregion

#pragma region Internal functions

void OSManager::OnStart() {
	GameLoopManager::Start();
}
void OSManager::OnUpdate() {
	GameLoopManager::Update();
}
void OSManager::OnButtonDown(int KeyCode) {
	GameLoopManager::OnKeyDown(KeyCode);
}
void OSManager::OnButtonUp(int KeyCode) {
	GameLoopManager::OnKeyUp(KeyCode);
}
void OSManager::SetMousePos(int MovementX, int MovementY) {
	GameLoopManager::SetMousePos(MovementX, MovementY);
}

#pragma endregion

#pragma region External functions

unsigned long*& OSManager::GetScreenColoursRef() {
	return Window::GetScreenColoursRef();
}

unsigned int OSManager::GetScreenWidth() {
	return ScreenWidth;
}
unsigned int OSManager::GetScreenHeight() {
	return ScreenHeight;
}
unsigned int OSManager::GetGameWidth() {
	return GameWidth;
}
unsigned int OSManager::GetGameHeight() {
	return GameHeight;
}

std::string OSManager::GetTitle() {
	return Window::GetWTitle();
}
void OSManager::SetTitle(const char* NewTitle) {
	Window::SetWTitle(NewTitle);
}

void OSManager::SetGameSize(unsigned int NewWidth, unsigned int NewHeight) {
	Window::SetGameSize(NewWidth, NewHeight);
}

#pragma endregion