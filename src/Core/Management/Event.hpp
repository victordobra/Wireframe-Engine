#pragma once

#include "Core/Types/Defines.hpp"
#include <vector>
#include <stdexcept>

namespace wfe {
	/// @brief A class abstracting an event, calling listeners when triggered.
	class Event {
	public:
		/// @brief The event listener callback type.
		/// @param userData The data passed to the listener for every event trigger.
		/// @param params The parameters to pass to the listener.
		/// @return The return value of the listener, which can be optionally queried.
		typedef void*(*ListenerCallback)(void* userData, void* params);

		/// @brief A structure containing the event's listener data.
		struct Listener {
			/// @brief The listener's callback function.
			ListenerCallback callback;
			/// @brief The data passed to the listener for every event trigger.
			void* userData;
		};
		
		/// @brief Creates an event with no listeners.
		Event() = default;
		/// @brief Copies the given event.
		/// @param other The event to copy.
		Event(const Event& other) = default;
		/// @brief Moves the given event's listeners.
		/// @param other The event whose listeners to move.
		Event(Event&& other) noexcept = default;

		/// @brief Copies the given event's listeners info this event.
		/// @param other The event whose listeners to copy.
		/// @return A reference to this event.
		Event& operator=(const Event& other) = default;
		/// @brief Moves the given event's listeners to this event.
		/// @param other The event whose listeners to move.
		/// @return A reference to this event.
		Event& operator=(Event&& other) noexcept = default;

		/// @brief Gets the list of the event's listeners.
		/// @return A vector of the event's listeners.
		const std::vector<Listener>& GetListeners() const {
			return this->listeners;
		}
		/// @brief Adds a listener to the event.
		/// @param listener The listener to add to the event.
		void AddListener(Listener listener) {
			this->listeners.push_back(listener);
		}
		/// @brief Removes the listener at the given index from the event.
		/// @param index The index of the listener to remove.
		void RemoveListener(size_t index) {
			if(index >= this->listeners.size())
				throw std::out_of_range("Index out of range!");

			this->listeners.erase(this->listeners.begin() + index);
		}
		/// @brief Removes the given listener from the event.
		/// @param listener The listener to remove from the event.
		void RemoveListener(Listener listener) {
			// Search for the listener in the vector
			for(auto iter = this->listeners.begin(); iter != this->listeners.end(); ++iter) {
				// Remove the current listener and exit the function, if found
				if(iter->callback == listener.callback && iter->userData == listener.userData) {
					this->listeners.erase(iter);
					return;
				}
			}

			// The listener wasn't found; throw an exception
			throw std::invalid_argument("Listener not found!");
		}
		/// @brief Removes all listeners from the event.
		void ClearListeners() {
			this->listeners.clear();
		}

		/// @brief Triggers the event, calling all listeners.
		/// @param params The parameters to pass to the listeners.
		/// @param returns An optional array to store the return values of the listeners. Ignored if null.
		void TriggerEvent(void* params, void** returns = nullptr) const {
			// Call each listener in the event
			for(size_t i = 0; i < this->listeners.size(); ++i) {
				// Call the listener and store the return value, if requested
				if(returns != nullptr) {
					returns[i] = this->listeners[i].callback(this->listeners[i].userData, params);
				} else {
					this->listeners[i].callback(this->listeners[i].userData, params);
				}
			}
		}

		/// @brief Destroys the event.
		~Event() = default;
	private:
		std::vector<Listener> listeners;
	};
}