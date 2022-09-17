#include "WireframeEngineEditor.hpp"
#include "ECS/EngineECS.hpp"

namespace wfe {
    // Callbacks
    static void LoadCallback() {
        // Load the scene
        GameObject::scene = new GameObject();
        GameObject::scene->Load("Scene.gameobj");
    }
    static void SaveCallback() {
        // Save the scene
        GameObject::scene->Save("Scene.gameobj");
    }
    WFE_EDITOR_SET_LOAD_CALLBACK(LoadCallback)
    WFE_EDITOR_SET_SAVE_CALLBACK(SaveCallback)
}