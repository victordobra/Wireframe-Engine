#include "OSManager.h"
#include "OSManagerInternal.h"

#include "BuildData.h"
#include "GameLoopManagerInternal.h"

#if BUILD_PLATFORM_WINDOWS
#include "Windows\Windows.h"

static unsigned int& ScreenWidth = WindowsWindow::GetScreenWidthRef();
static unsigned int& ScreenHeight = WindowsWindow::GetScreenHeightRef();
static unsigned int& GameWidth = WindowsWindow::GetGameWidthRef();
static unsigned int& GameHeight = WindowsWindow::GetGameHeightRef();
unsigned long*& OSMScreenColours = WindowsWindow::GetScreenColoursRef();

unsigned long*& OSManager::GetScreenColoursRef() {
	return WindowsWindow::GetScreenColoursRef();
}
std::string OSManager::GetTitle() {
	return WindowsWindow::GetWTitle();
}
void OSManager::SetTitle(const char* NewTitle) {
	WindowsWindow::SetWTitle(NewTitle);
}
void OSManager::SetGameSize(unsigned int NewWidth, unsigned int NewHeight) {
	WindowsWindow::SetGameSize(NewWidth, NewHeight);
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