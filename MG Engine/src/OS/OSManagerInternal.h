#pragma once

namespace OSManager {
	void OnStart();
	void OnUpdate();
	void OnButtonDown(int KeyCode);
	void OnButtonUp(int KeyCode);
	void SetMousePos(int NewX, int NewY);
}