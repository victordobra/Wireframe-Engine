#pragma once

#include "Core/Assets/Asset.hpp"
#include "Core/Types/Defines.hpp"
#include "Graphics/Main/MaterialManager.hpp"
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
		MaterialCollection(Program* program, uint64_t id = UINT64_T_MAX) : Asset(program, id) { }
		MaterialCollection(const MaterialCollection&) = delete;
		MaterialCollection(MaterialCollection&&) = delete;

		MaterialCollection& operator=(const MaterialCollection&) = delete;
		MaterialCollection& operator=(MaterialCollection&&) = delete;

		/// @brief Loads the material collection from a binary stream, using the final encoding.
		/// @param stream The stream to load the collection from.
		void Load(std::istream& stream) override;
		/// @brief Saves the material collection to a binary stream, using the final encoding.
		/// @param stream The stream to save the collection to.
		void Save(std::ostream& stream) const override;
		/// @brief Imports the material collection from a file.
		/// @param path The path to the file to import the collection from.
		void Import(const std::string& path) override;
		/// @brief Exports the material collection to a file.
		/// @param path The path to the file to export the collection to.
		void Export(const std::string& path) const override;
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
		WFE_ASSET_TYPE(MaterialCollection, { "mtl" })

		std::vector<Item> items;
	};
}