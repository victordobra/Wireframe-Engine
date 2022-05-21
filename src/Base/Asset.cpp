#include "Asset.hpp"

namespace mge {
    // Variables
    unordered_map<string, Asset*> Asset::assets = {};
    unordered_map<Asset*, string> Asset::assetLocations = {};
    unordered_map<size_t, AssetInfo> Asset::assetTypes = {};
    vector<pthread_t> Asset::assetThreads = {};

    void Asset::JoinThreads() {
        for(size_t i = 0; i < assetThreads.size(); i++)
            pthread_join(assetThreads[i], NULL);
    }

    // External functions
    void Asset::LoadFromFile(const string& filePath) {
        FileInput input(filePath);

        input.Get((char_t*)this, sizeof(*this));
        
        input.Close();
    }
    void Asset::SaveToFile(const string& filePath) {
        FileOutput output(filePath);

        output.WriteBuffer((char_t*)this, sizeof(*this));

        output.Close();
    }
}