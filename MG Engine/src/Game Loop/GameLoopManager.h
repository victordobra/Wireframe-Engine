#pragma once

namespace GameLoopManager {
	unsigned int GetScreenWidth();
	unsigned int GetScreenHeight();
	unsigned int GetGameWidth();
	unsigned int GetGameHeight();
	void SetGameWidth(unsigned int NewWidth);
	void SetGameHeight(unsigned int NewHeight);
	void SetGameSize(unsigned int NewWidth, unsigned int NewHeight);
	bool IsKeyDown(int KeyCode);
	bool IsKeyUp(int KeyCode);
	bool IsKeyPressed(int KeyCode);
	bool IsKeyReleased(int KeyCode);
	float GetDeltaTime();
}
