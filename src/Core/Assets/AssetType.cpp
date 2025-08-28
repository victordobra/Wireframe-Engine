#include "AssetType.hpp"
#include <cstring>
#include <stdexcept>

namespace wfe {
	// Asset type vector definition
	constinit size_t AssetType::assetTypeCount = 0;
	constinit AssetType AssetType::assetTypes[AssetType::MAX_ASSET_TYPE_COUNT];

	// Public functions
	const AssetType& AssetType::GetAssetType(const Asset* asset) {
		// Search for the asset type by the platform name
		const char* platformName = typeid(*asset).name();

		for(size_t i = 0; i != assetTypeCount; ++i) {
			if(!std::strcmp(assetTypes[i].platformName, platformName))
				return assetTypes[i];
		}

		// If not found, throw an exception
		throw std::invalid_argument("Asset type not found!");
	}
	const AssetType& AssetType::GetAssetTypeName(const std::string& name) {
		// Search for the asset type by name
		for(size_t i = 0; i != assetTypeCount; ++i) {
			if(assetTypes[i].name == name)
				return assetTypes[i];
		}

		// If not found, throw an exception
		throw std::invalid_argument("Asset type with name \"" + name + "\" not found!");
	}
	const AssetType& AssetType::GetAssetTypeExtension(const std::string& extension) {
		// Search for the asset type by extension
		for(size_t i = 0; i != assetTypeCount; ++i) {
			for(size_t j = 0; j != assetTypes[i].importExtensionCount; ++j)
				if(assetTypes[i].importExtensions[j] == extension)
					return assetTypes[i];
		}

		// If not found, throw an exception
		throw std::invalid_argument("Asset type with extension \"" + extension + "\" not found!");
	}
	void AssetType::RegisterAssetType(const AssetType& assetType) {
		// Check if the asset type is already registered
		for(size_t i = 0; i != assetTypeCount; ++i) {
			if(!std::strcmp(assetTypes[i].name, assetType.name))
				throw std::invalid_argument((std::string)"Asset type with name \"" + assetType.name + "\" is already registered!");
		}

		// Check if the asset type's extensions are already registered
		for(size_t i = 0; i != assetType.importExtensionCount; ++i) {
			for(size_t j = 0; j != assetTypeCount; ++j) {
				for(size_t k = 0; k != assetTypes[j].importExtensionCount; ++k)
					if(!std::strcmp(assetTypes[j].importExtensions[k], assetType.importExtensions[i]))
						throw std::invalid_argument((std::string)"Asset type with extension \"" + assetType.importExtensions[i] + "\" is already registered!");
			}
		}

		// Check if the max asset type count was reached
		if(assetTypeCount == MAX_ASSET_TYPE_COUNT)
			throw std::length_error("Maximum asset type count of " + std::to_string(MAX_ASSET_TYPE_COUNT) + " was reached!");

		// Register the new asset type
		assetTypes[assetTypeCount++] = assetType;
	}
}