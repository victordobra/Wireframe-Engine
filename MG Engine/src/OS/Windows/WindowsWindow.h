#pragma once
#include <string>

namespace WindowsWin {
	unsigned long*& GetScreenColoursRef();
	unsigned int& GetScreenWidthRef();
	unsigned int& GetScreenHeightRef();
	unsigned int& GetGameWidthRef();
	unsigned int& GetGameHeightRef();
	void SetGameSize(unsigned int NewWidth, unsigned int NewHeight);
	std::string GetWTitle();
	void SetWTitle(const char* NewString);
}