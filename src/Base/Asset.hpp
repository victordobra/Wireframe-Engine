#pragma once

#include "Core.hpp"
#include <pthread.h>
#include <unistd.h>
#include <typeinfo>

namespace mge {
    struct AssetInfo;

    class Asset {
    public:
        static unordered_map<size_t, AssetInfo> assetTypes;
        static vector<pthread_t> assetThreads;

        static void JoinThreads();

        Asset() = default;
        Asset(const Asset&) = delete;
        Asset(Asset&&) noexcept = delete;

        Asset& operator=(const Asset&) = delete;
        Asset& operator=(Asset&&) noexcept = delete;

        virtual void LoadFromFile(const string& filePath);
        virtual void SaveToFile(const string& filePath);

        template<class T>
        static Asset* LoadAssetFromFile(const string& filePath) { 
            if(!assets.count(filePath)) {
                Asset* newAsset = dynamic_cast<Asset*>(new T());

                if(!newAsset)
                    console::OutFatalError("Failed to convert the asset type!", 1);

                newAsset->LoadFromFile(filePath);
                assets.insert(filePath, newAsset);
                assetLocations.insert(newAsset, filePath);
            }
            return dynamic_cast<T*>(assets[filePath]);
        }
        template<class T>
        static string GetAssetLocation(const T* originalAsset) {
            Asset* asset = (Asset*)dynamic_cast<const Asset*>(originalAsset);
            
            if(!asset)
                console::OutFatalError("Failed to convert the asset type!", 1);

            return assetLocations[asset];
        }
        static void DeleteAssets() {
            using node = unordered_map<string, Asset*>::node;
            node** bucket = assets.data();
            size_t bucketCount = assets.bucket_count();

            for(size_t i = 0; i < bucketCount; ++i) {
                node* currentNode = *(bucket++);

                while(currentNode != nullptr) {
                    delete currentNode->value;
                    currentNode = currentNode->next;
                }
            }
        }

        virtual ~Asset() = default;
    private:
        static Asset*& LoadAssetPrivate(const string& filePath, bool8_t& initialize);

        static unordered_map<string, Asset*> assets;
        static unordered_map<Asset*, string> assetLocations;
    };

    struct AssetInfo {
        Asset*(*create)(const string&);
    };
}

#define MGE_ASSET(type) \
namespace { \
    void* Create ## type ## Asset(void* pParams) { \
        while(!mge::Asset::assetTypes.bucket_count() && !mge::Asset::assetThreads.data()) \
            sleep(0.05); \
        mge::AssetInfo info; \
        info.create = mge::Asset::LoadAssetFromFile<type>; \
        mge::Asset::assetTypes[typeid(type).hash_code()] = info; \
        return 0; \
    } \
    struct type ## AssetCreator { \
        type ## AssetCreator() { \
            pthread_t thread; \
            pthread_create(&thread, NULL, Create ## type ## Asset, NULL); \
            mge::Asset::assetThreads.push_back(thread); \
        } \
    }; \
} 
