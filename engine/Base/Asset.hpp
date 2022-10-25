#pragma once

#include "Core.hpp"
#include <typeinfo>

namespace wfe {
#ifdef NDEBUG
    #define WFE_ASSET_LOCATION "assets/"
#else
    #define WFE_ASSET_LOCATION "../assets/"
#endif
    class Asset;

    struct AssetType {
        string name{};
        uint64_t hashCode{};
        uint64_t ptrHashCode{};

        Asset*(*create)(){};

        constexpr AssetType() = default;
        AssetType(const AssetType&) = default;
        AssetType(AssetType&&) noexcept = default;

        AssetType& operator=(const AssetType&) = default;
        AssetType& operator=(AssetType&&) noexcept = default;

        bool8_t operator==(const AssetType& other) const {
            return hashCode == other.hashCode;
        }
        bool8_t operator< (const AssetType& other) const {
            return hashCode < other.hashCode;
        }
        bool8_t operator> (const AssetType& other) const {
            return hashCode > other.hashCode;
        }
        bool8_t operator<=(const AssetType& other) const {
            return hashCode <= other.hashCode;
        }
        bool8_t operator>=(const AssetType& other) const {
            return hashCode >= other.hashCode;
        }

        constexpr ~AssetType() = default;
    };

    class Asset {
    public:
        static map<uint64_t, AssetType> assetTypes;

        string name;
        string location;
        AssetType* assetType;

        Asset();
        Asset(const Asset&) = delete;
        Asset(Asset&&) noexcept = delete;

        Asset& operator=(const Asset&) = delete;
        Asset& operator=(Asset&&) noexcept = delete;

        /// @brief Loads the asset from a file.
        /// @param fileLocation The location of the file relative to the assets folder.
        void Load  (const string& fileLocation);
        /// @brief Save the asset to a file.
        /// @param fileLocation The location of the file relative to the assets folder.
        void Save  (const string& fileLocation);
        /// @brief Imports the asset from an external file.
        /// @param fileLocation The location of the file relative to the assets folder.
        void Import(const string& fileLocation);

        /// @brief Deletes every asset. Called at the end of the program.
        static void DeleteAllAssets();
        /// @brief Returns a vector with every asset.
        static vector<Asset*> GetAssets() {
            return assets;
        }
        /// @brief Returns the asset with the specified name.
        /// @param name The name of the asset.
        /// @return A pointer to the asset, or a nullptr if it doesn't exist.
        static Asset* GetAssetWithName(const string& name);
        /// @brief Returns the asset with the specified location.
        /// @param location The location of the asset.
        /// @return A pointer to the asset, or a nullptr if it doesn't exist.
        static Asset* GetAssetWithLocation(const string& location);
        /// @brief Returns the asset with the specified location or creates it if it doesn't exist
        /// @param location The location of the asset.
        template<class T>
        static T* GetOrCreateAssetWithLocation(const string& location) {
            // Assert that the imputted type must be an asset
            uint64_t hashCode = WFE_TYPE_ID(T).Hash64();
            pair<uint64_t, AssetType>* assetType = assetTypes.find(hashCode);
            assert((assetType != assetTypes.end()) && "The inputted type must be an asset!");

            // Loop through every asset, checking for their locations
            for(size_t i = 0; i < assets.size(); ++i)
                if(assets[i]->location == location)
                    return (T*)assets[i];
            
            // Create the asset
            T* asset = dynamic_cast<T*>(assetType->val2.create());
            asset->Load(location);
            return asset;
        }

        /// @brief Called when the editor properties need to be drawn.
        virtual void DrawEditorWindow() { }

        virtual ~Asset();
    protected:
        /// @brief Called when the asset needs to be loaded.
        virtual void LoadFromFile  (const string& fileLocation) { }
        /// @brief Called when the asset needs to be saved.
        virtual void SaveToFile    (const string& fileLocation) { }
        /// @brief Called when the asset needs to be imported from an external file.
        virtual void ImportFromFile(const string& fileLocation) { }
    private:
        static vector<Asset*> assets;
    };
}

#define WFE_ASSET(type) \
namespace { \
    /* Function that creates an asset of the specified type and returns it. */ \
    wfe::Asset* Create ## type ## Asset() { \
        wfe::Asset* asset = dynamic_cast<wfe::Asset*>(new type()); \
        if(!asset) \
            console::OutFatalError("Failed to convert object to asset type!", 1); \
        asset->assetType = &wfe::Asset::assetTypes[WFE_TYPE_ID(type).Hash64()]; \
        return asset; \
    } \
    struct type ## AssetTypeInitializer { \
        /* Creates the asset type and adds it to the array */ \
        type ## AssetTypeInitializer() { \
            wfe::AssetType assetType{}; \
            assetType.name = #type; \
            assetType.hashCode = WFE_TYPE_ID(type).Hash64(); \
            assetType.ptrHashCode = WFE_TYPE_ID(type*).Hash64(); \
            assetType.create = Create ## type ## Asset; \
 \
            wfe::Asset::assetTypes.insert({ assetType.hashCode, assetType }); \
        } \
    }; \
 \
    type ## AssetTypeInitializer type ## AssetTypeInit; \
}
