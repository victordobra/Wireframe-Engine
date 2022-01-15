#pragma once

namespace mge {
	void GLMStart();
	void GLMUpdate();
	void GLMOnKeyDown(int keyCode);
	void GLMOnKeyUp(int keyCode);
	void GLMSetMousePos(size_t newX, size_t newY);
}