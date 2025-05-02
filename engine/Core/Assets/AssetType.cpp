#include "AssetType.hpp"
#include <stdexcept>

namespace wfe {
	// Asset type vector definition
	constinit std::vector<AssetType> AssetType::assetTypes;

	// Public functions
	const AssetType& AssetType::GetAssetTypeName(const std::string& name) {
		// Search for the asset type by name
		for(const AssetType& assetType : assetTypes) {
			if(assetType.name == name)
				return assetType;
		}

		// If not found, throw an exception
		throw std::invalid_argument("Asset type with name \"" + name + "\" not found!");
	}
	const AssetType& AssetType::GetAssetTypeExtension(const std::string& extension) {
		// Search for the asset type by extension
		for(const AssetType& assetType : assetTypes) {
			if(assetType.importExtensions.count(extension))
				return assetType;
		}

		// If not found, throw an exception
		throw std::invalid_argument("Asset type with extension \"" + extension + "\" not found!");
	}
	void AssetType::RegisterAssetType(const AssetType& assetType) {
		// Check if the asset type is already registered
		for(const AssetType& existingAssetType : assetTypes) {
			if(existingAssetType.name == assetType.name)
				throw std::invalid_argument("Asset type with name \"" + assetType.name + "\" is already registered!");
		}

		// Check if the asset type's extensions are already registered
		for(const std::string& extension : assetType.importExtensions) {
			for(const AssetType& existingAssetType : assetTypes) {
				if(existingAssetType.importExtensions.count(extension))
					throw std::invalid_argument("Asset type with extension \"" + extension + "\" is already registered!");
			}
		}

		// Register the new asset type
		assetTypes.push_back(assetType);
	}
}