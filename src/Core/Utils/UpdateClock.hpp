#pragma once

#include "Core/Types/Defines.hpp"
#include <chrono>
#include <thread>

namespace wfe {
	/// @brief An update clock class, which times continuous update loops.
	class UpdateClock {
	public:
		/// @brief Creates an update clock.
		UpdateClock() = default;
		UpdateClock(const UpdateClock&) = delete;
		UpdateClock(UpdateClock&&) = delete;

		UpdateClock& operator=(const UpdateClock&) = delete;
		UpdateClock& operator=(UpdateClock&&) = delete;

		/// @brief Starts the update clock.
		void StartClock() {
			prevTickStart = std::chrono::high_resolution_clock::now();
		}
		/// @brief Updates the clock, setting the new tick duration.
		void TickClock() {
			// Calculate the previous tick's duration
			std::chrono::high_resolution_clock::time_point newTickStart = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = newTickStart - prevTickStart;

			// Check if the tick duration is too low
			if(duration.count() < minTickDuration) {
				// Get the time point at the end of the thread sleep
				std::chrono::nanoseconds minTickDurationNS = std::chrono::nanoseconds{ (int64_t)(minTickDuration * 1000000000.0f) };
				std::chrono::high_resolution_clock::time_point tickEnd = prevTickStart + minTickDurationNS;

				// Wait for the remaining time
				while(std::chrono::high_resolution_clock::now() < tickEnd)
					std::this_thread::sleep_for(std::chrono::seconds{ 0 });

				// Update the new tick start and duration
				newTickStart = std::chrono::high_resolution_clock::now();
				duration = newTickStart - prevTickStart;
			}

			// Set the tick duration, in seconds, and update the previous tick start
			prevTickDuration = duration.count();
			prevTickStart = newTickStart;
		}

		/// @brief Gets the duration of the previous tick.
		/// @return The duration of the previous tick, in seconds.
		float GetPrevTickDuration() const {
			return prevTickDuration;
		}
		/// @brief Gets the minimum tick duration.
		/// @return The minimum tick duration, in seconds.
		float GetMinTickDuration() const {
			return minTickDuration;
		}
		/// @brief Sets the minimum tick duration.
		/// @param newMinTickDuration The new minimum tick duration, in seconds.
		void SetMinTickDuration(float newMinTickDuration) {
			minTickDuration = newMinTickDuration;
		}

		/// @brief Destroys the update clock.
		~UpdateClock() = default;
	private:
		std::chrono::high_resolution_clock::time_point prevTickStart;
		float prevTickDuration = 0.0f;
		float minTickDuration = 0.0f;
	};
}