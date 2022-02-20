#include "EngineTime.h"
#include "EngineTimeInternal.h"

#include <ctime>

namespace mge {
	//Variables
	float32_t deltaTime = 0.f;
	size_t frameIndex = 0;

	clock_t start;

	//Internal functions
	void StartFrame() {
		start = clock();
	}
	void EndFrame() {
		clock_t end = clock();
		clock_t dtClock = end - start;

		deltaTime = (float)dtClock / CLOCKS_PER_SEC;
		frameIndex++;
	}

	float32_t GetDeltaTime() {
		return deltaTime;
	}
	size_t GetFrameIndex() {
		return frameIndex;
	}
}