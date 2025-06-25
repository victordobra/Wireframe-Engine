#pragma once

#include "Core/Assets/Asset.hpp"
#include "Core/Types/Defines.hpp"

namespace wfe {
	/// @brief A class representing a text file asset.
	class TextFile : public Asset {
	public:
		/// @brief Creates a new text file.
		/// @param program The program that owns the asset.
		/// @param id The asset's ID, or UINT64_T_MAX if the asset has no ID.
		TextFile(Program* program, uint64_t id = UINT64_T_MAX) : Asset(program, id) { }
		TextFile(const TextFile&) = delete;
		TextFile(TextFile&&) = delete;

		TextFile& operator=(const TextFile&) = delete;
		TextFile& operator=(TextFile&&) = delete;

		/// @brief Loads the text file from a binary stream, using the final encoding.
		/// @param stream The stream to load the text file from.
		void Load(std::istream& stream) override;
		/// @brief Saves the text file to a binary stream, using the final encoding.
		/// @param stream The stream to save the text file to.
		void Save(std::ostream& stream) const override;
		/// @brief Imports the text file from a file.
		/// @param path The path to the file to import the text file from.
		void Import(const std::string& path) override;
		/// @brief Exports the text file to a file.
		/// @param path The path to the file to export the text file to.
		void Export(const std::string& path) const override;
		/// @brief Gets the text file's dependencies.
		/// @return The text file's dependencies.
		std::vector<Asset*> GetDependencies() const override {
			return {};
		}

		/// @brief Gets the text file's size.
		/// @return The text file's size.
		size_t GetSize() const {
			return data.size();
		}
		/// @brief Gets the text file's data.
		/// @return A pointer to the text file's data.
		char* GetData() {
			return data.data();
		}
		/// @brief Gets the text file's data.
		/// @return A constant pointer to the text file's data.
		const char* GetData() const {
			return data.data();
		}

		/// @brief Destroys the text file.
		~TextFile() = default;
	private:
		WFE_ASSET_TYPE(TextFile, { "txt" })

		std::vector<char> data;
	};
}