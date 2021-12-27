#pragma once
#include<string>

namespace OSManager {
	unsigned long*& GetScreenColoursRef();

	unsigned int GetScreenWidth();
	unsigned int GetScreenHeight();
	unsigned int GetGameWidth();
	unsigned int GetGameHeight();

	const char* GetTitle();
	void SetTitle(const char* NewTitle);

	void SetGameSize(unsigned int NewWidth, unsigned int NewHeight);
}

