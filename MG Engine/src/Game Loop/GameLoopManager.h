#pragma once

namespace GameLoopManager {
	unsigned long*& GetScreenColoursRef();
	unsigned int GetScreenWidth();
	unsigned int GetScreenHeight();
	unsigned int GetGameWidth();
	unsigned int GetGameHeight();
	void SetGameWidth(unsigned int NewWidth);
	void SetGameHeight(unsigned int NewHeight);
	void SetGameSize(unsigned int NewWidth, unsigned int NewHeight);
}
