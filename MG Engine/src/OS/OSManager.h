#pragma once
#include<string>

namespace mge {
	size_t OSMGetScreenWidth();
	size_t OSMGetScreenHeight();
	size_t OSMGetGameWidth();
	size_t OSMGetGameHeight();

	const char* OSMGetTitle();
	void OSMSetTitle(const char* newTitle);

	void OSMSetGameSize(size_t newWidth, size_t newHeight);
}

