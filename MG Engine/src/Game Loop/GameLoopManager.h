#pragma once

namespace mge {
	size_t GLMGetScreenWidth();
	size_t GLMGetScreenHeight();
	size_t GLMGetGameWidth();
	size_t GLMGetGameHeight();

	void GLMSetGameWidth(size_t newWidth);
	void GLMSetGameHeight(size_t newHeight);
	void GLMSetGameSize(size_t newWidth, size_t newHeight);

	bool GLMIsKeyDown(int keyCode);
	bool GLMIsKeyUp(int keyCode);

	bool GLMIsKeyPressed(int keyCode);
	bool GLMIsKeyReleased(int keyCode);

	float GLMGetDeltaTime();
}