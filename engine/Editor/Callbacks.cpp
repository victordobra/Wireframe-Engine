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
        uint64_t hashCode = WFE_TYPE_ID(GameObject).Hash64();
        auto& assetType = Asset::assetTypes[hashCode];
        GameObject::scene = (GameObject*)assetType.create();
        GameObject::scene->Load("Scene.gameobj");
    }
    static void SaveCallback() {
        // Save every asset
        for(auto* asset : Asset::GetAssets())
            if(asset->location.length())
                asset->Save(asset->location);
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