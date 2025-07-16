#pragma once

#include "Core/Types/Defines.hpp"
#include "Asset.hpp"
#include "AssetManager.hpp"
#include <filesystem>
#include <vector>

namespace wfe {
	class Program;

	/// @brief A class representing a directory of assets.
	class AssetDirectory {
	public:
		/// @brief Creates a new blank asset directory.
		/// @param program The program that owns the asset directory.
		/// @param path The path of the asset directory.
		AssetDirectory(Program* program, const std::filesystem::path& path) : program(program), path(path.lexically_normal()) { }
		AssetDirectory(const AssetDirectory&) = delete;
		AssetDirectory(AssetDirectory&&) = delete;

		AssetDirectory& operator=(const AssetDirectory&) = delete;
		AssetDirectory& operator=(AssetDirectory&&) = delete;

		/// @brief Loads the asset directory from its path, using the final encoding for the directory metadata and all assets.
		void Load();
		/// @brief Saves the asset to its path, using the final encoding for the directory metadata and all assets.
		/// @param saveAssets True if the assets should be saved, false if only the directory metadata should be saved.
		void Save(bool saveAssets = true) const;
		/// @brief Imports the asset directory from its path.
		void Import();
		/// @brief Exports the asset directory to its path.
		/// @param exportAssets True if the assets should be exported, false if only the directory metadata should be exported.
		void Export(bool exportAssets = true) const;

		/// @brief Gets the program that owns the asset directory.
		/// @return The program that owns the asset directory.
		Program* GetProgram() const {
			return program;
		}
		/// @brief Gets the asset directory's path.
		/// @return The asset directory's path.
		const std::filesystem::path& GetPath() const {
			return path;
		}
		/// @brief Gets the assets in the asset directory.
		/// @return A reference to the vector containing the directory's assets.
		std::vector<Asset*>& GetAssets() {
			return assets;
		}
		/// @brief Gets the assets in the asset directory.
		/// @return A const reference to the vector containing the directory's assets.
		const std::vector<Asset*>& GetAssets() const {
			return assets;
		}

		/// @brief Sets the asset directory's path.
		/// @param newPath The asset directory's new path.
		/// @param updateAssetPaths True if the paths of all assets in the directory should be update to be relative to the new directory path, otherwise false.
		void SetPath(const std::filesystem::path& newPath, bool updateAssetPaths = true);

		/// @brief Destroys the asset directory and all of its assets.
		~AssetDirectory();
	private:
		void GetLoadInterval(size_t index, const std::unordered_map<uint64_t, size_t>& indices, const std::vector<std::vector<uint64_t>>& dependencies, std::vector<std::pair<size_t, size_t>>& loadIntervals);
		void GetAssetLoadOrder(const std::vector<uint64_t>& ids, const std::vector<std::vector<uint64_t>>& dependencies, std::vector<std::vector<size_t>>& loadStartOrder, std::vector<std::vector<size_t>>& loadEndOrder);

		Program* program;

		std::filesystem::path path;
		std::vector<Asset*> assets;
	};
}