#include "GameLoopManager.h"
#include "GameLoopManagerInternal.h"
#include "OSManager.h"
#include "EngineMath.h"

#include <windows.h>

unsigned long*& ScreenColours = OSManager::GetScreenColoursRef();
static unsigned int ScreenWidth;
static unsigned int ScreenHeight;
static unsigned int GameWidth;
static unsigned int GameHeight;

void GameLoopManager::Start() {
	ScreenWidth = OSManager::GetScreenWidth();
	ScreenHeight = OSManager::GetScreenHeight();
	GameWidth = OSManager::GetGameWidth();
	GameHeight = OSManager::GetGameHeight();
}
void GameLoopManager::Update() {
	for (unsigned int i = 0; i < GameWidth; i++)
		for (unsigned int j = 0; j < GameHeight; j++)
			ScreenColours[i + j * GameWidth] = i * 255 / GameWidth + j * 255 / GameHeight * 0x000100 + (GameHeight - j) * 255 / GameHeight * 0x010000;
}

unsigned long*& GameLoopManager::GetScreenColoursRef() {
	return OSManager::GetScreenColoursRef();
}
unsigned int GameLoopManager::GetScreenWidth() {
	return ScreenWidth;
}
unsigned int GameLoopManager::GetScreenHeight() {
	return ScreenHeight;
}
unsigned int GameLoopManager::GetGameWidth() {
	return GameWidth;
}
unsigned int GameLoopManager::GetGameHeight() {
	return GameHeight;
}
void GameLoopManager::SetGameWidth(unsigned int NewWidth) {
	GameWidth = NewWidth;
	OSManager::SetGameSize(GameWidth, GameHeight);
}
void GameLoopManager::SetGameHeight(unsigned int NewHeight) {
	GameHeight = NewHeight;
	OSManager::SetGameSize(GameWidth, GameHeight);
}
void GameLoopManager::SetGameSize(unsigned int NewWidth, unsigned int NewHeight) {
	GameWidth = NewWidth;
	GameHeight = NewHeight;
	OSManager::SetGameSize(GameWidth, GameHeight);
}