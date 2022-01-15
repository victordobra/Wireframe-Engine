#pragma once

namespace mge {
	void OSMOnStart();
	void OSMOnUpdate();
	void OSMOnButtonDown(int keyCode);
	void OSMOnButtonUp(int keyCode);
	void OSMSetMousePos(size_t newX, size_t newY);
}