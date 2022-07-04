#pragma once

#include "Core.hpp"
#include <typeinfo>

namespace wfe {
#ifdef NDEBUG
    #define ASSET_PATH "./assets/"
#else
    #define ASSET_PATH "../assets/"
#endif
    class Asset;

    struct AssetType {
        string name{};
        uint64_t hashCode{};
        uint64_t ptrHashCode{};

        Asset*(*create)(){};

        constexpr AssetType() = default;
        constexpr AssetType(const AssetType&) = default;
        constexpr AssetType(AssetType&&) noexcept = default;

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
        static const size_t MAX_ASSET_TYPE_COUNT = 1024;
        static AssetType assetTypes[MAX_ASSET_TYPE_COUNT];
        static size_t assetTypeCount;

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
        void Load(const string& fileLocation);
        /// @brief Save the asset to a file.
        /// @param fileLocation The location of the file relative to the assets folder.
        void Save(const string& fileLocation);

        /// @brief Loads every asset. Called at the start of the program.
        static void SortAssetTypes();
        /// @brief Deletes every asset. Called at the end of the program.
        static void DeleteAllAssets();
        /// @brief Returns the asset type with the specified hash code.
        /// @param hashCode The hash code of the type.
        /// @return A pointer to the asset type, or a nullptr if it doesn't exist.
        static AssetType* GetAssetType(uint64_t hashCode);
        /// @brief Returns the asset type with its pointer's hash code equal to the given hash code.
        /// @param hashCode The hash code of the type pointer.
        /// @return A pointer to the asset type, or a nullptr if it doesn't exist.
        static AssetType* GetAssetPtrType(uint64_t hashCode);
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
            AssetType* assetType = GetAssetType(hashCode);
            assert(assetType && "The inputted type must me an asset!");

            // Loop through every asset, checking for their locations
            for(size_t i = 0; i < assets.size(); ++i)
                if(assets[i]->location == location)
                    return (T*)assets[i];
            
            // Create the asset
            T* asset = dynamic_cast<T*>(assetType->create());
            asset->Load(location);
            return asset;
        }

        virtual ~Asset();
    protected:
        virtual void LoadFromFile(const string& fileLocation);
        virtual void SaveToFile  (const string& fileLocation);
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
        asset->assetType = wfe::Asset::GetAssetType(WFE_TYPE_ID(type).Hash64()); \
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
            wfe::Asset::assetTypes[wfe::Asset::assetTypeCount++] = assetType; \
        } \
    }; \
 \
    type ## AssetTypeInitializer type ## AssetTypeInit; \
}
