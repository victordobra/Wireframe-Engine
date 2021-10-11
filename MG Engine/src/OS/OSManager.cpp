#include "OSManager.h"
#include "OSManagerInternal.h"

#include "Windows\WindowsWindow.h"

#include "BuildData.h"
#include "GameLoopManagerInternal.h"

#if BUILD_PLATFORM_WINDOWS
static unsigned int& ScreenWidth = WindowsWin::GetScreenWidthRef();
static unsigned int& ScreenHeight = WindowsWin::GetScreenHeightRef();
static unsigned int& GameWidth = WindowsWin::GetGameWidthRef();
static unsigned int& GameHeight = WindowsWin::GetGameHeightRef();
unsigned long*& OSMScreenColours = WindowsWin::GetScreenColoursRef();

unsigned long*& OSManager::GetScreenColoursRef() {
	return WindowsWin::GetScreenColoursRef();
}
std::string OSManager::GetTitle() {
	return WindowsWin::GetWTitle();
}
void OSManager::SetTitle(const char* NewTitle) {
	WindowsWin::SetWTitle(NewTitle);
}
void OSManager::SetGameSize(unsigned int NewWidth, unsigned int NewHeight) {
	WindowsWin::SetGameSize(NewWidth, NewHeight);
}
#endif

#pragma region Internal functions

void OSManager::OnStart() {
	GameLoopManager::Start();
}
void OSManager::OnUpdate() {
	GameLoopManager::Update();
}
void OSManager::OnButtonDown(int KeyCode) {
	
}
void OSManager::OnButtonUp(int KeyCode) {
	
}

#pragma endregion
#pragma region External functions


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

#pragma endregion