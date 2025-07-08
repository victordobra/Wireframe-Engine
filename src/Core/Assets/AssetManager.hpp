#pragma once

#include "Core/Types/Defines.hpp"
#include "Asset.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <future>

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

		/// @brief Loads all assets from a directory.
		/// @param path The path to the directory to load assets from.
		void LoadDirectory(const std::string& path);
		/// @brief Saves all assets to the given directory.
		/// @param path The path to the directory to save assets to. It must be a previously loaded directory.
		/// @param saveAssets True if the assets should be saved, false if only the directory structure should be saved.
		void SaveDirectory(const std::string& path, bool saveAssets = false) const;
		/// @brief Imports all assets from a directory.
		/// @param path The path to the directory to import assets from.
		void ImportDirectory(const std::string& path);
		/// @brief Exports all assets to the given directory.
		/// @param path The path to the directory to export assets to. It must be a previously imported directory.
		/// @param exportAssets True if the assets should be exported, false if only the directory structure should be exported.
		void ExportDirectory(const std::string& path, bool exportAssets = false) const;
		/// @brief Unloads all assets from a directory.
		/// @param path The path to the directory to unload assets from.
		void UnloadDirectory(const std::string& path);

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
		Asset* GetAsset(const std::string& path) const {
			// Lock the asset mutex
			uint32_t locked = 0;
			while(!assetsMutex.compare_exchange_strong(locked, 1))
				locked = 0;
			
			// Get the asset from the path map
			auto iter = assetsPath.find(path);
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

		void GetLoadInterval(size_t index, const std::unordered_map<uint64_t, size_t>& indices, const std::vector<std::vector<uint64_t>>& dependencies, std::vector<std::pair<size_t, size_t>>& loadIntervals);
		void GetAssetLoadOrder(const std::vector<uint64_t>& ids, const std::vector<std::vector<uint64_t>>& dependencies, std::vector<std::vector<size_t>>& loadStartOrder, std::vector<std::vector<size_t>>& loadEndOrder);

		Program* program;

		std::unordered_map<uint64_t, Asset*> assetsID;
		std::unordered_map<std::string, Asset*> assetsPath;
		std::unordered_map<std::string, std::vector<Asset*>> directories;

		mutable atomic_uint32_t assetsMutex;
	};
}