#pragma once

#include "Core/Assets/Asset.hpp"
#include "Core/Types/Defines.hpp"
#include "Graphics/Main/Resources/Material.hpp"
#include <string>
#include <vector>

namespace wfe {
	/// @brief A class representing a collection of materials, each with a different name.
	class MaterialCollection : public Asset {
	public:
		/// @brief A struct defining an item in the material collection.
		struct Item {
			/// @brief The material's name.
			std::string name;
			/// @brief A pointer to the material object.
			Material* material;
		};

		/// @brief Creates a new blank material collection. The collection must be subsequentally loaded or imported.
		/// @param program The program that owns the asset.
		/// @param id The asset's ID, or UINT64_T_MAX if the asset has no ID.
		/// @param path The asset file's path. Left blank if the asset has no path.
		MaterialCollection(Program* program, uint64_t id = UINT64_T_MAX, const std::filesystem::path& path = "") : Asset(program, id, path) { }
		MaterialCollection(const MaterialCollection&) = delete;
		MaterialCollection(MaterialCollection&&) = delete;

		MaterialCollection& operator=(const MaterialCollection&) = delete;
		MaterialCollection& operator=(MaterialCollection&&) = delete;

		/// @brief Loads the material collection from its path, using the final encoding.
		void Load() override;
		/// @brief Saves the material collection to its path, using the final encoding.
		void Save() const override;
		/// @brief Imports the material collection from its path.
		void Import() override;
		/// @brief Exports the material collection to its path.
		void Export() const override;
		/// @brief Gets the material collection's dependencies.
		/// @return The collection's dependencies.
		std::vector<Asset*> GetDependencies() const override;

		/// @brief Gets the material collection's items.
		/// @return A reference to the vector containing the material collection's items.
		std::vector<Item>& GetItems() {
			return items;
		}
		/// @brief Gets the material collection's items.
		/// @return A const reference to the vector containing the material collection's items.
		const std::vector<Item>& GetItems() const {
			return items;
		}

		/// @brief Destroys the material colection.
		~MaterialCollection();
	private:
		WFE_ASSET_TYPE(MaterialCollection, ".mtl")

		std::vector<Item> items;
	};
}