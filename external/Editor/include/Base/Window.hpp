#pragma once

#include "Core.hpp"
#include "imgui.hpp"

namespace wfe::editor {
    struct WindowType {
        static map<string, WindowType>* windowTypes;

        string name;
        void(*render)();
        bool8_t open;
    };

    typedef void(*LoadCallback)();
    typedef void(*SaveCallback)();

    /// @brief Loads the current workspace dir and the other recent dirs.
    void LoadWorkspace();
    /// @brief Saves the current workspace dir and the other recent dirs.
    void SaveWorkspace();
    /// @brief Renders every window.
    void RenderWindows();

    /// @brief Returns the load callback.
    LoadCallback GetLoadCallback();
    /// @brief Sets the load callback.
    void SetLoadCallback(LoadCallback newLoadCallback);
    /// @brief Returns the save callback.
    SaveCallback GetSaveCallback();
    /// @brief Sets the save callback.
    void SetSaveCallback(SaveCallback newSaveCallback);

    /// @brief Returns the workspace dir.
    string GetWorkspaceDir();
    /// @brief Sets the workspace dir.
    void SetWorkspaceDir(const string& newWorkspaceDir, bool8_t removeFromRecents = true);
    // @brief Closes the current workspace.
    void CloseWorkspace();
}

// Macro for creating the window type
#define WFE_EDITOR_WINDOW_TYPE(winName, winRender) \
namespace { \
    /* Struct for creating the window type */ \
    struct EditorWindowInit { \
        EditorWindowInit() { \
            wfe::editor::WindowType windowType; \
 \
            windowType.name = winName; \
            windowType.render = winRender; \
            windowType.open = false; \
 \
            wfe::editor::WindowType::windowTypes->insert({ windowType.name, windowType }); \
        } \
    }; \
 \
    EditorWindowInit initWindow; \
}
// Macro for setting the load callback
#define WFE_EDITOR_SET_LOAD_CALLBACK(callback) \
namespace { \
    /* Struct for setting the load callback */ \
    struct LoadCallbackSetter { \
        LoadCallbackSetter() { \
            wfe::editor::SetLoadCallback(callback); \
        } \
    }; \
    LoadCallbackSetter loadCallbackSetter; \
}
// Macro for setting the save callback
#define WFE_EDITOR_SET_SAVE_CALLBACK(callback) \
namespace { \
    /* Struct for setting the save callback */ \
    struct SaveCallbackSetter { \
        SaveCallbackSetter() { \
            wfe::editor::SetSaveCallback(callback); \
        } \
    }; \
    SaveCallbackSetter saveCallbackSetter; \
}
