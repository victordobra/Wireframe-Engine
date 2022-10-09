#include "WireframeEngineEditor.hpp"
#include "ECS/EngineECS.hpp"

namespace wfe {
    // Callbacks
    static void LoadCallback() {
        // Create the Vulkan objects
        Component::AddAssetTypeProperties();

        CreateDevice();
        CreateSampler();

        // Load the scene
        GameObject::scene = new GameObject();
        GameObject::scene->Load("Scene.gameobj");
    }
    static void SaveCallback() {
        // Save the scene
        GameObject::scene->Save("Scene.gameobj");
    }
    static void CloseCallback() {
        // Delete every asset
        Asset::DeleteAllAssets();
        
        // Delete the sampler
        DeleteSampler();
    }
    WFE_EDITOR_SET_LOAD_CALLBACK(LoadCallback)
    WFE_EDITOR_SET_SAVE_CALLBACK(SaveCallback)
    WFE_EDITOR_SET_CLOSE_CALLBACK(CloseCallback)
}