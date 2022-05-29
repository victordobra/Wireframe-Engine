#include "Time.hpp"
#include <sys/time.h>

namespace mge {
    // Variables
    timespec deltaTimeStart;
    timespec deltaTimeEnd;

    float32_t deltaTime{0.f};
    float32_t runTime{0.f};
    size_t frameIndex{0};
    
    void StartUpdateTimer() {
        clock_gettime(CLOCK_MONOTONIC, &deltaTimeStart);
    }
    void StopUpdateTimer() {
        clock_gettime(CLOCK_MONOTONIC, &deltaTimeEnd);

        timespec dif;
        dif.tv_sec = deltaTimeEnd.tv_sec - deltaTimeStart.tv_sec;
        dif.tv_nsec = deltaTimeEnd.tv_nsec - deltaTimeStart.tv_nsec;
        
        deltaTime = dif.tv_sec + dif.tv_nsec * .000001f;
        runTime += deltaTime;
        ++frameIndex;
    }

    float32_t GetDeltaTime() {
        return deltaTime;
    }
    float32_t GetProgramRuntime() {
        return runTime;
    }
    size_t GetFrameIndex() {
        return frameIndex;
    }
}