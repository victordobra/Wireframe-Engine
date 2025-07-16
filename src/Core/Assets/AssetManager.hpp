#pragma once

#include "Core/Types/Defines.hpp"
#include "Asset.hpp"
#include <filesystem>
#include <unordered_map>
#include <vector>

namespace wfe {
	class Program;
	class Asset;

	/// @brief A class that manages assets.
	class AssetManager {
	public:
		/// @brief Creates a new asset manager.
		/// @param program The program that owns the asset manager.
		AssetManager(Program* program) : program(program) { };
		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;

		AssetManager& operator=(const AssetManager&) = delete;
		AssetManager& operator=(AssetManager&&) = delete;

		/// @brief Gets the asset with the given ID.
		/// @param id The ID of the asset to get.
		/// @return A pointer to the asset with the given ID, or nullptr if the asset does not exist.
		Asset* GetAsset(uint64_t id) const {
			// Lock the asset mutex
			uint32_t locked = 0;
			while(!assetsMutex.compare_exchange_strong(locked, 1))
				locked = 0;
			
			// Get the asset from the ID map
			auto iter = assetsID.find(id);
			Asset* asset;
			if(iter != assetsID.end()) {
				asset = iter->second;
			} else {
				asset = nullptr;
			}

			// Unlock the mutex and return the asset
			assetsMutex = 0;
			return asset;
		}
		/// @brief Gets the asset with the given path.
		/// @param path The path of the asset to get.
		/// @return A pointer to the asset with the given path, or nullptr if the asset does not exist.
		Asset* GetAsset(const std::filesystem::path& path) const {
			// Lock the asset mutex
			uint32_t locked = 0;
			while(!assetsMutex.compare_exchange_strong(locked, 1))
				locked = 0;
			
			// Get the asset from the path map
			auto iter = assetsPath.find(path.lexically_normal());
			Asset* asset;
			if(iter != assetsPath.end()) {
				asset = iter->second;
			} else {
				asset = nullptr;
			}

			// Unlock the mutex and return the asset
			assetsMutex = 0;
			return asset;
		}
		/// @brief Gets the asset manager's owned assets.
		/// @return The map of the asset manager's owned assets.
		std::vector<Asset*> GetAssets() const {
			// Lock the asset mutex
			uint32_t locked = 0;
			while(!assetsMutex.compare_exchange_strong(locked, 1))
				locked = 0;

			// Create the asset vector
			std::vector<Asset*> assets;
			assets.reserve(assetsID.size());

			// Add all assets from the ID map
			for(const std::pair<const uint64_t, Asset*> assetPair : assetsID)
				assets.push_back(assetPair.second);
			
			// Unlock the asset mutex and return the vector
			assetsMutex = 0;
			return assets;
		}

		/// @brief Destroys the asset manager.
		~AssetManager() = default;
	private:
		friend Asset;

		Program* program;

		std::unordered_map<uint64_t, Asset*> assetsID;
		std::unordered_map<std::filesystem::path, Asset*> assetsPath;

		mutable atomic_uint32_t assetsMutex;
	};
}