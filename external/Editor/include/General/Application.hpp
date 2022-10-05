#pragma once

#include "Core.hpp"
#include "imgui.hpp"

namespace wfe::editor {
    /// @brief Holds an event type.
    typedef enum : uint32_t {
        /// @brief Triggered when the application is closed.
        EVENT_TYPE_EXIT,
        /// @brief Triggered when a key is pressed or released.
        EVENT_TYPE_KEY,
        /// @brief Triggered when any character is inputted.
        EVENT_TYPE_INPUT_CHAR,
        /// @brief Triggered when a mouse button is pressed or released.
        EVENT_TYPE_MOUSE_BUTTON,
        /// @brief Triggered when the mouse is moved.
        EVENT_TYPE_MOUSE_MOVE,
        /// @brief Triggered when the window is moved.
        EVENT_TYPE_WINDOW_MOVE,
        /// @brief Triggered when the window is resized.
        EVENT_TYPE_WINDOW_RESIZE,
        /// @brief Triggered when the window needs to be renamed.
        EVENT_TYPE_WINDOW_RENAME,
        /// @brief The total number of event types.
        EVENT_TYPE_COUNT
    } EventType;

    /// @brief The exit event info.
    struct ExitEventInfo {
        /// @brief The final return code of the platform.
        int32_t returnCode;
    };
    /// @brief The key event info.
    struct KeyEventInfo {
        /// @brief Whether the key was pressed or released.
        bool8_t pressed;
        /// @brief The ImGui key code of the key.
        ImGuiKey keyCode;
    };
    /// @brief The input char event info.
    struct InputCharEventInfo {
        /// @brief The inputed UTF8 string.
        string inputString;
    };
    /// @brief The mouse button event info.
    struct MouseButtonEventInfo {
        /// @brief Whether the mouse button was pressed or released
        bool8_t pressed;
        /// @brief The ImGui mouse button
        ImGuiMouseButton mouseButton;
    };
    /// @brief The mouse move event info.
    struct MouseMoveEventInfo {
        /// @brief The X position of the mouse.
        uint32_t mouseX;
        /// @brief The Y position of the mouse.
        uint32_t mouseY;
        /// @brief The scroll wheel's delta.
        float32_t scrollWheelDelta;
    };
    /// @brief The window move event info.
    struct WindowMoveEventInfo {
        /// @brief The X position of the window.
        uint32_t windowX;
        /// @brief The Y position of the window.
        uint32_t windowY;
    };
    /// @brief The window resize event info. 
    struct WindowResizeEventInfo {
        /// @brief The width of the window.
        uint32_t windowWidth;
        /// @brief The height of the window.
        uint32_t windowHeight;
    };
    /// @brief The window rename event info.
    struct WindowRenameEventInfo {
        /// @brief The new name of the window.
        string newName;
    };

    /// @brief Holds info about an event.
    struct Event {
        /// @brief The type of event.
        EventType eventType;
        /// @brief A pointer to any of the event info structs. Should be casted in corespondence with the event type.
        void* eventInfo;
    };

    /// @brief Gets all events that triggered in the current frame.
    /// @return A vector with every event.
    vector<Event> GetEvents();
    /// @brief Gets all events with the specified type that triggered in the current frame.
    /// @param type The type of event to look for.
    /// @return A vector with every wanted event.
    vector<Event> GetEventsOfType(EventType type);

    /// @brief Adds an event to the queue to be processed next frame.
    /// @param event The event to add to the queue.
    void AddEvent(Event event);

    /// @brief Exits the application.
    /// @param returnCode The final return code.
    void CloseApplication(int32_t returnCode);
    /// @brief Whether the program is running inside the editor.
    /// @return If the program is running insize the editor or not.
    bool8_t IsInsideEditor();

    /// @brief Gets the width of the main window.
    /// @return The width of the main window.
    size_t GetMainWindowWidth();
    /// @brief Gets the height of the main window.
    /// @return The height of the main window.
    size_t GetMainWindowHeight();

    /// @brief Gets the name of the main window.
    /// @return The name of the main window.
    string GetMainWindowName();
    /// @brief Sets the name of the main window.
    /// @param newName The new name of the main window.
    void SetMainWindowName(const string& newName);
}