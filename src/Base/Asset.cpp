#include "Asset.hpp"

namespace mge {
    // Variables
    constinit AssetType Asset::assetTypes[MAX_ASSET_TYPE_COUNT];
    constinit size_t Asset::assetTypeCount = 0;

    vector<Asset*> Asset::assets = {};

    // Member functions
    Asset::Asset() : name(""), location("") {
        // Add itself to the asset vector
        assets.push_back(this);

        // Find the asset's hash code
        size_t hashCode = typeid(*this).hash_code();

        // Binary search for the appropriate type
        size_t pos = 0, step = MAX_ASSET_TYPE_COUNT >> 1;

        while(step) {
            if(pos + step < assetTypeCount && assetTypes[pos + step].hashCode <= hashCode)
                pos += step;
            step >>= 1;
        }

        // Set the asset type
        type = assetTypes + pos;
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

        input.Get((char_t*)this, sizeof(*this));

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
        mge::console::OutFatalError("Failed to find asset in asset vector!", 1);
    }

    // Static functions
    void Asset::LoadAllAssets() {
        // Sort the type array with the set class
        set<AssetType> typeSet;

        for(size_t i = 0; i < assetTypeCount; ++i)
            typeSet.insert(assetTypes[i]);

        size_t ind = 0;
        for(const auto& type : typeSet)
            assetTypes[ind++] = type;
    }
    void Asset::DeleteAllAssets() {
        // Save the asset vector
        FileOutput output((string)ASSET_PATH + "Assets.assets");

        uint64_t size64 = assets.size();
        output.WriteBuffer((char_t*)&size64, sizeof(uint64_t));

        for(uint64_t i = 0; i < size64; ++i) {
            output.WriteBuffer((char_t*)&assets[i]->type->hashCode, sizeof(size_t));

            uint64_t pathLength = assets[i]->location.size();
            output.WriteBuffer((char_t*)&pathLength, sizeof(uint64_t));
            output.WriteBuffer((char_t*)assets[i]->location.c_str(), pathLength * sizeof(char_t));
        }

        output.Close();

        // Delete every asset
        for(uint64_t i = 0; i < size64; ++i)
            delete assets[0];
    }
    AssetType* Asset::GetAssetType(size_t hashCode) { 
        // Binary search for the type
        size_t pos = 0, step = MAX_ASSET_TYPE_COUNT >> 1;

        while(step) {
            if(pos + step < assetTypeCount && assetTypes[pos + step].hashCode <= hashCode)
                pos += step;
            step >>= 1;
        }

        // If the type's hash code is the specified one, return a pointer to the type, otherwhise return a nullptr
        if(assetTypes[pos].hashCode == hashCode)
            return assetTypes + pos;
        return nullptr;
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