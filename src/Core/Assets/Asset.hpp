#pragma once

#include "Core/Assets/AssetType.hpp"
#include <filesystem>
#include <vector>

namespace wfe {
	class Program;

	/// @brief A base class for all assets.
	class Asset {
	public:
		/// @brief Creates a new asset.
		/// @param program The program that owns the asset.
		/// @param id The asset's ID, or UINT64_T_MAX if the asset has no ID.
		/// @param path The asset file's path. Left blank if the asset has no path.
		Asset(Program* program, uint64_t id = UINT64_T_MAX, const std::filesystem::path& path = "");
		Asset(const Asset&) = delete;
		Asset(Asset&&) = delete;

		Asset& operator=(const Asset&) = delete;
		Asset& operator=(Asset&&) = delete;

		/// @brief Loads the asset from its path, using the final encoding.
		virtual void Load() = 0;
		/// @brief Saves the asset to its path, using the final encoding.
		virtual void Save() const = 0;
		/// @brief Imports the asset from its path.
		virtual void Import() = 0;
		/// @brief Exports the asset to its path.
		virtual void Export() const = 0;
		/// @brief Gets the asset's dependencies.
		/// @return A vector of the asset's dependencies.
		virtual std::vector<Asset*> GetDependencies() const = 0;

		/// @brief Gets the program that owns the asset.
		/// @return The program that owns the asset.
		Program* GetProgram() const { 
			return program;
		}
		/// @brief Gets the asset's ID.
		/// @return The asset's ID, or UINT64_MAX if the asset has no ID.
		uint64_t GetID() const {
			return id;
		}
		/// @brief Gets the asset's path, as used by the asset manager.
		/// @return The asset's path, as used by the asset manager.
		const std::filesystem::path& GetPath() const {
			return path;
		}

		/// @brief Sets the asset's ID.
		/// @param newID The new asset ID.
		void SetID(uint64_t newID);
		/// @brief Sets the asset's path which the asset manager will use.
		/// @param newPath The new asset path.
		void SetPath(const std::filesystem::path& newPath);

		/// @brief Destroys the asset.
		virtual ~Asset();
	private:
		Program* program;
		uint64_t id;
		std::filesystem::path path;
	};
}
