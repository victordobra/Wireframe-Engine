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
			auto it = assets.find(id);
			if (it != assets.end())
				return it->second;
			return nullptr;
		}
		/// @brief Gets the asset manager's owned assets.
		/// @return The map of the asset manager's owned assets.
		const std::unordered_map<uint64_t, Asset*>& GetAssets() const {
			return assets;
		}

		/// @brief Destroys the asset manager.
		~AssetManager() = default;
	private:
		friend Asset;

		struct Directory {
			std::vector<std::string> paths;
			std::vector<Asset*> assets;
		};

		void GetLoadInterval(size_t index, const std::unordered_map<uint64_t, size_t>& indices, const std::vector<std::vector<uint64_t>>& dependencies, std::vector<std::pair<size_t, size_t>>& loadIntervals);
		void GetAssetLoadOrder(const std::vector<uint64_t>& ids, const std::vector<std::vector<uint64_t>>& dependencies, std::vector<std::vector<size_t>>& loadStartOrder, std::vector<std::vector<size_t>>& loadEndOrder);

		Program* program;
		std::unordered_map<uint64_t, Asset*> assets;
		std::unordered_map<std::string, Directory> directories;
	};
}