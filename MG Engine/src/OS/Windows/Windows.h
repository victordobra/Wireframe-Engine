#pragma once
#include <string>

namespace WindowsWindow {
	unsigned long*& GetScreenColoursRef();
	unsigned int& GetScreenWidthRef();
	unsigned int& GetScreenHeightRef();
	unsigned int& GetGameWidthRef();
	unsigned int& GetGameHeightRef();
	void SetGameSize(unsigned int NewWidth, unsigned int NewHeight);
	char* GetWTitle();
	void SetWTitle(const char* NewString);
}