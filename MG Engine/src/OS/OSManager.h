#pragma once
#include<string>

namespace OSManager {
	unsigned long*& GetScreenColoursRef();
	std::string GetTitle();
	void SetTitle(const char* NewTitle);
	unsigned int GetScreenWidth();
	unsigned int GetScreenHeight();
	unsigned int GetGameWidth();
	unsigned int GetGameHeight();
	void SetGameSize(unsigned int NewWidth, unsigned int NewHeight);
}

