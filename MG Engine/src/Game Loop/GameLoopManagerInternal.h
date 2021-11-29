#pragma once
namespace GameLoopManager {
	void Start();
	void Update();
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void SetMousePos(int NewX, int NewY);
}