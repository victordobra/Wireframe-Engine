#pragma once

#include "MaterialCollection.hpp"
#include "Core/Assets/Asset.hpp"
#include "Core/Types/Defines.hpp"
#include "Graphics/Main/MaterialManager.hpp"
#include "Graphics/Main/RenderMesh.hpp"
#include <string>
#include <vector>

namespace wfe {
	/// @brief A class representing a render object, made of multiple mashes, each with an attributed material.
	class RenderObject : public Asset {
	public:
		/// @brief A struct defining an item in the render object.
		struct Item {
			/// @brief The item's name.
			std::string name;
			/// @brief The render mesh of the item.
			RenderMesh* mesh;
			/// @brief The material associated to the render mesh, or nullptr if the material does not exist.
			Material* material;
		};

		/// @brief Creates a new blank render object. The object must be subsequentally loaded or imported.
		/// @param program The program that owns the asset.
		/// @param id The asset's ID, or UINT64_T_MAX if the asset has no ID.
		RenderObject(Program* program, uint64_t id = UINT64_T_MAX) : Asset(program, id) { }
		RenderObject(const RenderObject&) = delete;
		RenderObject(RenderObject&&) = delete;

		RenderObject& operator=(const RenderObject&) = delete;
		RenderObject& operator=(RenderObject&&) = delete;

		/// @brief Loads the render object from a binary stream, using the final encoding.
		/// @param stream The stream to load the object from.
		void Load(std::istream& stream) override;
		/// @brief Saves the render object to a binary stream, using the final encoding.
		/// @param stream The stream to save the object to.
		void Save(std::ostream& stream) const override;
		/// @brief Imports the render object from a file.
		/// @param path The path to the file to import the object from.
		void Import(const std::string& path) override;
		/// @brief Exports the render object to a file.
		/// @param path The path to the file to export the object to.
		void Export(const std::string& path) const override;
		/// @brief Gets the render object's dependencies.
		/// @return The object's dependencies.
		std::vector<Asset*> GetDependencies() const override {
			if(materialCollection) {
				return { materialCollection };
			} else {
				return {};
			}
		}

		/// @brief Gets the material collection corresponding to this render object.
		/// @brief The material collection corresponding to this render object.
		MaterialCollection* GetMaterialCollection() const {
			return materialCollection;
		}
		/// @brief Gets the render object's items.
		/// @return A reference to the vector containing the material collection's items.
		std::vector<Item>& GetItems() {
			return items;
		}
		/// @brief Gets the render object's items.
		/// @return A const reference to the vector containing the material collection's items.
		const std::vector<Item>& GetItems() const {
			return items;
		}
		
		/// @brief Destroys the render object.
		~RenderObject();
	private:
		WFE_ASSET_TYPE(RenderObject, { "obj" })

		MaterialCollection* materialCollection;
		std::vector<Item> items;
	};
}