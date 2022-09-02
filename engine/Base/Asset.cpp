#include "Asset.hpp"

namespace wfe {
    // Variables
    constinit map<uint64_t, AssetType> Asset::assetTypes;

    vector<Asset*> Asset::assets = {};

    // Member functions
    Asset::Asset() : name(""), location("") {
        // Add itself to the asset vector
        assets.push_back(this);
    }

    void Asset::Load(const string& fileLocation) {
        // Find the name of the asset from the file location
        location = fileLocation;

        const char_t* namePtr = location.c_str();
        const char_t* temp = strchr(namePtr, '/');
        while(temp) {
            namePtr = temp + 1;
            temp = strchr(namePtr, '/');
        }

        name = namePtr;

        // Call the internal load function
        LoadFromFile((string)ASSET_PATH + location);
    }
    void Asset::Save(const string& fileLocation) {
        // Find the name of the asset from the file location
        location = fileLocation;

        const char_t* namePtr = location.c_str();
        const char_t* temp = strchr(namePtr, '/');
        while(temp) {
            namePtr = temp + 1;
            temp = strchr(namePtr, '/');
        }

        name = namePtr;

        // Call the internal save function
        SaveToFile((string)ASSET_PATH + fileLocation);
    }

    void Asset::LoadFromFile(const string& fileLocation) {
        FileInput input(fileLocation);

        input.ReadBuffer((char_t*)this, sizeof(*this));

        input.Close();
    }
    void Asset::SaveToFile  (const string& fileLocation) {
        FileOutput output(fileLocation);

        output.WriteBuffer((char_t*)this, sizeof(*this));

        output.Close();
    }

    Asset::~Asset() {
        // Find the asset in the asset vector
        for(size_t i = 0; i < assets.size(); ++i)
            if(assets[i] == this) {
                assets.erase(assets.begin() + i);
                return;
            }
        console::OutFatalError((string)"Failed to find asset in asset vector!", 1);
    }

    // Static functions
    void Asset::DeleteAllAssets() {
        // Delete every asset
        while(assets.size())
            delete assets[0];
    }
    Asset* Asset::GetAssetWithName(const string& name) { 
        // Loop through every asset, checking for their names
        for(size_t i = 0; i < assets.size(); ++i)
            if(assets[i]->name == name)
                return assets[i];
        // Return a nullptr
        return nullptr;
    }
    Asset* Asset::GetAssetWithLocation(const string& location) { 
        // Loop through every asset, checking for their locations
        for(size_t i = 0; i < assets.size(); ++i)
            if(assets[i]->location == location)
                return assets[i];
        // Return a nullptr
        return nullptr;
    }
}