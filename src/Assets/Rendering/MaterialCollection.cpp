#include "MaterialCollection.hpp"
#include "Core/Utils/BinaryIO.hpp"
#include "Main/Program.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace wfe {
	// Virtual function definitions
	void MaterialCollection::Load() {
		// Destroy all previous items
		for(size_t i = 0; i != items.size(); ++i)
			delete items[i].material;
		items.clear();

		// Open the file stream for reading
		std::ifstream stream(GetPath(), std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open material collection file \"" + GetPath().string() + "\" for reading!");

		// Query the default texture
		ImageTexture* defaultTexture = GetProgram()->GetEngineGraphics()->GetMaterialManager()->GetDefaultImageTexture();
	
		// Read the number of items
		uint64_t itemCount = BinaryReadUint64(stream);
		items.resize(itemCount);

		for(size_t i = 0; i != itemCount; ++i) {
			// Read the material's name
			uint64_t nameLength = BinaryReadUint64(stream);
			items[i].name.resize(nameLength);
			stream.read(items[i].name.data(), nameLength);

			// Read the material's data
			Material::MaterialData materialData;

			materialData.ambientColor.x = BinaryReadFloat(stream);
			materialData.ambientColor.y = BinaryReadFloat(stream);
			materialData.ambientColor.z = BinaryReadFloat(stream);

			materialData.diffuseColor.x = BinaryReadFloat(stream);
			materialData.diffuseColor.y = BinaryReadFloat(stream);
			materialData.diffuseColor.z = BinaryReadFloat(stream);

			materialData.specularColor.x = BinaryReadFloat(stream);
			materialData.specularColor.y = BinaryReadFloat(stream);
			materialData.specularColor.z = BinaryReadFloat(stream);

			materialData.specularExponent = BinaryReadFloat(stream);

			// Read the material texture IDs
			uint64_t ambientTextureID = BinaryReadUint64(stream);
			uint64_t diffuseTextureID = BinaryReadUint64(stream);
			uint64_t specularTextureID = BinaryReadUint64(stream);
			uint64_t specularExponentMapID = BinaryReadUint64(stream);

			// Set the material's textures
			Material::MaterialTextures materialTextures;

			if(ambientTextureID == UINT64_T_MAX) {
				materialTextures.ambientTexture = defaultTexture;
			} else {
				materialTextures.ambientTexture = (ImageTexture*)GetProgram()->GetAssetManager()->GetAsset(ambientTextureID);
				if(!materialTextures.ambientTexture)
					throw std::runtime_error("Invalid image texture ID stored in material collection file!");
			}
			if(diffuseTextureID == UINT64_T_MAX) {
				materialTextures.diffuseTexture = defaultTexture;
			} else {
				materialTextures.diffuseTexture = (ImageTexture*)GetProgram()->GetAssetManager()->GetAsset(diffuseTextureID);
				if(!materialTextures.diffuseTexture)
					throw std::runtime_error("Invalid image texture ID stored in material collection file!");
			}
			if(specularTextureID == UINT64_T_MAX) {
				materialTextures.specularTexture = defaultTexture;
			} else {
				materialTextures.specularTexture = (ImageTexture*)GetProgram()->GetAssetManager()->GetAsset(diffuseTextureID);
				if(!materialTextures.specularTexture)
					throw std::runtime_error("Invalid image texture ID stored in material collection file!");
			}
			if(specularExponentMapID == UINT64_T_MAX) {
				materialTextures.specularExponentMap = defaultTexture;
			} else {
				materialTextures.specularExponentMap = (ImageTexture*)GetProgram()->GetAssetManager()->GetAsset(diffuseTextureID);
				if(!materialTextures.specularExponentMap)
					throw std::runtime_error("Invalid image texture ID stored in material collection file!");
			}
			
			// Create the material
			items[i].material = new Material(GetProgram()->GetEngineGraphics()->GetMaterialManager(), materialData, materialTextures);
		}

		// Close the file stream
		stream.close();
	}
	void MaterialCollection::Save() const {
		// Open the file stream for writing
		std::ofstream stream(GetPath(), std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open material collection file \"" + GetPath().string() + "\" for writing!");

		// Write the number of items
		BinaryWriteUint64(stream, items.size());

		// Write all of the items
		for(size_t i = 0; i != items.size(); ++i) {
			// Write the material's name
			BinaryWriteUint64(stream, items[i].name.size());
			stream.write(items[i].name.data(), items[i].name.size());

			// Write the material's data
			const Material::MaterialData& materialData = items[i].material->GetData();

			BinaryWriteFloat(stream, materialData.ambientColor.x);
			BinaryWriteFloat(stream, materialData.ambientColor.y);
			BinaryWriteFloat(stream, materialData.ambientColor.z);

			BinaryWriteFloat(stream, materialData.diffuseColor.x);
			BinaryWriteFloat(stream, materialData.diffuseColor.y);
			BinaryWriteFloat(stream, materialData.diffuseColor.z);

			BinaryWriteFloat(stream, materialData.specularColor.x);
			BinaryWriteFloat(stream, materialData.specularColor.y);
			BinaryWriteFloat(stream, materialData.specularColor.z);

			BinaryWriteFloat(stream, materialData.specularExponent);

			// Write the material texture IDs
			const Material::MaterialTextures& materialTextures = items[i].material->GetTextures();

			BinaryWriteUint64(stream, materialTextures.ambientTexture->GetID());
			BinaryWriteUint64(stream, materialTextures.diffuseTexture->GetID());
			BinaryWriteUint64(stream, materialTextures.specularTexture->GetID());
			BinaryWriteUint64(stream, materialTextures.specularExponentMap->GetID());
		}

		// Close the file stream
		stream.close();
	}
	void MaterialCollection::Import() {
		// Destroy all previous items
		for(size_t i = 0; i != items.size(); ++i)
			delete items[i].material;
		items.clear();
	
		// Open the file stream for reading
		std::ifstream stream(GetPath());
		if(!stream)
			throw std::runtime_error("Failed to open material collection file \"" + GetPath().string() + "\" for reading!");
		
		// Read all of the file's lines
		std::vector<std::string> lines;

		while(stream) {
			// Read the current line
			std::string line;
			for(char ch; stream.get(ch);) {
				// Exit the loop if the character is a line break
				if(ch == '\n')
					break;
				
				// Add the character to the line
				line += ch;
			}

			// Get all whitespace characters at the start or end of the line
			size_t lineStart = line.find_first_not_of(" \f\n\r\t\v");
			size_t lineEnd = line.find_last_not_of(" \f\n\r\t\v");

			// Skip the current line if it is full of whitespace
			if(lineStart == std::string::npos || lineEnd == std::string::npos)
				continue;

			// Skip the current line if it is a comment
			if(line[lineStart] == '#')
				continue;
			
			// Add the new line, with all whitespace trimmed, to the vector
			lines.push_back(line.substr(lineStart, lineEnd - lineStart + 1));
		}

		// Close the file stream
		stream.close();

		// Get the directory the material collection file is in
		std::filesystem::path fileDir = GetPath().lexically_normal().parent_path();

		// Save the current material's info
		std::string currentName;
		Material::MaterialData currentData;
		Material::MaterialTextures currentTextures;

		// Query the default texture
		ImageTexture* defaultTexture = GetProgram()->GetEngineGraphics()->GetMaterialManager()->GetDefaultImageTexture();

		// Parse every line in the file
		for(const std::string& line : lines) {
			// Create an input string stream
			std::istringstream strStream(line);

			// Read the first keyword
			std::string keyword;
			strStream >> keyword;

			if(keyword == "newmtl") {
				// Add the previous material to the item array, if it exists
				if(!currentName.empty())
					items.push_back({ currentName, new Material(GetProgram()->GetEngineGraphics()->GetMaterialManager(), currentData, currentTextures) });
				
				// Reset the material info
				currentData.ambientColor = VEC3F_ONE;
				currentData.diffuseColor = VEC3F_ONE;
				currentData.specularColor = VEC3F_ONE;
				currentData.specularExponent = 32.0f;

				currentTextures.ambientTexture = defaultTexture;
				currentTextures.diffuseTexture = defaultTexture;
				currentTextures.specularTexture = defaultTexture;
				currentTextures.specularExponentMap = defaultTexture;

				// Read the material's name
				strStream >> currentName;
			} else if(keyword == "Ka") {
				// Read the ambient color's new values
				strStream >> currentData.ambientColor.x >> currentData.ambientColor.y >> currentData.ambientColor.z;
			} else if(keyword == "Kd") {
				// Read the diffuse color's new values
				strStream >> currentData.diffuseColor.x >> currentData.diffuseColor.y >> currentData.diffuseColor.z;
			} else if(keyword == "Ks") {
				// Read the specular color's new values
				strStream >> currentData.specularColor.x >> currentData.specularColor.y >> currentData.specularColor.z;
			} else if(keyword == "Ns") {
				// Read the specular exponent's new value
				strStream >> currentData.specularExponent;
			} else if(keyword == "map_Ka") {
				// Read the image file name
				std::string imageRelPath;
				strStream >> imageRelPath;
				
				// Get the image with the given path
				std::filesystem::path imagePath = (fileDir / imageRelPath).lexically_normal();
				currentTextures.ambientTexture = dynamic_cast<wfe::ImageTexture*>(GetProgram()->GetAssetManager()->GetAsset(imagePath));
				if(!currentTextures.ambientTexture)
					throw std::runtime_error("Image file with path \"" + imagePath.string() + "\" not found!");
			} else if(keyword == "map_Kd") {
				// Read the image file name
				std::string imageRelPath;
				strStream >> imageRelPath;
				
				// Get the image with the given path
				std::filesystem::path imagePath = (fileDir / imageRelPath).lexically_normal();
				currentTextures.diffuseTexture = dynamic_cast<wfe::ImageTexture*>(GetProgram()->GetAssetManager()->GetAsset(imagePath));
				if(!currentTextures.diffuseTexture)
					throw std::runtime_error("Image file with path \"" + imagePath.string() + "\" not found!");
			} else if(keyword == "map_Ks") {
				// Read the image file name
				std::string imageRelPath;
				strStream >> imageRelPath;
				
				// Get the image with the given path
				std::filesystem::path imagePath = (fileDir / imageRelPath).lexically_normal();
				currentTextures.specularTexture = dynamic_cast<wfe::ImageTexture*>(GetProgram()->GetAssetManager()->GetAsset(imagePath));
				if(!currentTextures.specularTexture)
					throw std::runtime_error("Image file with path \"" + imagePath.string() + "\" not found!");
			} else if(keyword == "map_Ns") {
				// Read the image file name
				std::string imageRelPath;
				strStream >> imageRelPath;
				
				// Get the image with the given path
				std::filesystem::path imagePath = (fileDir / imageRelPath).lexically_normal();
				currentTextures.specularExponentMap = dynamic_cast<wfe::ImageTexture*>(GetProgram()->GetAssetManager()->GetAsset(imagePath));
				if(!currentTextures.specularExponentMap)
					throw std::runtime_error("Image file with path \"" + imagePath.string() + "\" not found!");
			}
		}

		// Add the last material to the item array, if it exists
		if(!currentName.empty())
			items.push_back({ currentName, new Material(GetProgram()->GetEngineGraphics()->GetMaterialManager(), currentData, currentTextures) });
	}
	void MaterialCollection::Export() const {
		// Open the file stream for writing
		std::ofstream stream(GetPath());
		if(!stream)
			throw std::runtime_error("Failed to open material collection file \"" + GetPath().string() + "\" for writing!");

		// Set the stream's floating point precision
		stream << std::fixed << std::setprecision(6);
		
		// Write an automatic comment to the file
		stream << "# " << WFE_ENGINE_NAME << " version " << WFE_ENGINE_VERSION_MAJOR << '.' << WFE_ENGINE_VERSION_MINOR << '.' << WFE_ENGINE_VERSION_PATCH << '\n';
		stream << "# Automatically generated MTL file\n\n";

		// Get the directory the material collection file is in
		std::filesystem::path fileDir = GetPath().lexically_normal().parent_path();

		// Write every material's info to the file
		for(size_t i = 0; i != items.size(); ++i) {
			// Write the item's name
			stream << "newmtl " << items[i].name << '\n';

			// Write the material's data
			const wfe::Material::MaterialData data = items[i].material->GetData();

			stream << "\tKa " << data.ambientColor.x << ' ' << data.ambientColor.y << ' ' << data.ambientColor.z << '\n';
			stream << "\tKd " << data.diffuseColor.x << ' ' << data.diffuseColor.y << ' ' << data.diffuseColor.z << '\n';
			stream << "\tKs " << data.specularColor.x << ' ' << data.specularColor.y << ' ' << data.specularColor.z << '\n';
			stream << "\tNs " << data.specularExponent << '\n';

			stream << '\n';

			// Write the material's textures
			const Material::MaterialTextures textures = items[i].material->GetTextures();
			ImageTexture* defaultTexture = GetProgram()->GetEngineGraphics()->GetMaterialManager()->GetDefaultImageTexture();

			if(textures.ambientTexture != defaultTexture) {
				std::filesystem::path ambientTexturePath = textures.ambientTexture->GetPath().lexically_relative(fileDir);
				stream << "\tmap_Ka " << ambientTexturePath.string() << '\n';
			}
			if(textures.diffuseTexture != defaultTexture) {
				std::filesystem::path diffuseTexturePath = textures.diffuseTexture->GetPath().lexically_relative(fileDir);
				stream << "\tmap_Kd " << diffuseTexturePath.string() << '\n';
			}
			if(textures.specularTexture != defaultTexture) {
				std::filesystem::path specularTexturePath = textures.specularTexture->GetPath().lexically_relative(fileDir);
				stream << "\tmap_Kd " << specularTexturePath.string() << '\n';
			}
			if(textures.specularExponentMap != defaultTexture) {
				std::filesystem::path specularExponentMapPath = textures.specularExponentMap->GetPath().lexically_relative(fileDir);
				stream << "\tmap_Kd " << specularExponentMapPath.string() << '\n';
			}

			stream << '\n';
		}

		// Close the stream
		stream.close();
	}
	std::vector<Asset*> MaterialCollection::GetDependencies() const {
		// Loop through all images and get all unique assets
		std::unordered_set<Asset*> dependencies;
		for(size_t i = 0; i != items.size(); ++i) {
			// Add all of the current material's textures
			dependencies.insert(items[i].material->GetTextures().ambientTexture);
			dependencies.insert(items[i].material->GetTextures().diffuseTexture);
			dependencies.insert(items[i].material->GetTextures().specularTexture);
			dependencies.insert(items[i].material->GetTextures().specularExponentMap);
		}

		// Move all dependencies to a vector
		std::vector<Asset*> depVector;
		depVector.reserve(dependencies.size());

		for(Asset* dep : dependencies)
			depVector.push_back(dep);

		return depVector;
	}

	// Public functions
	MaterialCollection::~MaterialCollection() {
		// Destroy all member materials
		for(size_t i = 0; i != items.size(); ++i)
			delete items[i].material;
	}
}