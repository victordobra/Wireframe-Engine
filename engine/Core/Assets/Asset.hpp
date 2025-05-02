#pragma once

#include "Core/Assets/AssetType.hpp"
#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace wfe {
	class Program;

	/// @brief A base class for all assets.
	class Asset {
	public:
		/// @brief Creates a new asset.
		/// @param program The program that owns the asset.
		/// @param id The asset's ID, or UINT64_T_MAX if the asset has no ID.
		Asset(Program* program, uint64_t id = UINT64_T_MAX) : program(program), id(id) { }
		Asset(const Asset&) = delete;
		Asset(Asset&&) = delete;

		Asset& operator=(const Asset&) = delete;
		Asset& operator=(Asset&&) = delete;

		/// @brief Loads the asset from a binary stream, using the final encoding.
		/// @param stream The stream to load the asset from.
		virtual void Load(std::istream& stream) = 0;
		/// @brief Saves the asset to a binary stream, using the final encoding.
		/// @param stream The stream to save the asset to.
		virtual void Save(std::ostream& stream) const = 0;
		/// @brief Imports the asset from a file.
		/// @param path The path to the file to import the asset from.
		virtual void Import(const std::string& path) = 0;
		/// @brief Exports the asset to a file.
		/// @param path The epath to the file to export the asset to.
		virtual void Export(const std::string& path) const = 0;
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

		/// @brief Destroys the asset.
		virtual ~Asset() = default;
	private:
		Program* program;
		uint64_t id;
	};
}
