#pragma once

#include "Core/Management/TypeName.hpp"
#include "Core/Types/Defines.hpp"
#include <set>
#include <string>
#include <vector>

namespace wfe {
	class Asset;
	class Program;

	/// @brief A structure that defines an asset type.
	struct AssetType {
	public:
		/// @brief The asset constructor function type.
		typedef Asset*(*Constructor)(Program* program, uint64_t id);

		/// @brief Gets all registered asset types.
		/// @return A vector of all registered asset types.
		static const std::vector<AssetType>& GetAssetTypes() {
			return assetTypes;
		}
		/// @brief Gets the asset type with the given name.
		/// @param name The asset type's name.
		/// @return A reference to the asset type with the given name.
		static const AssetType& GetAssetTypeName(const std::string& name);
		/// @brief Gets the asset type which supports the given file extension.
		/// @param extension The file extension to check for.
		/// @return A reference to the asset type which supports the given file extension.
		static const AssetType& GetAssetTypeExtension(const std::string& extension);
		/// @brief Registers a new asset type.
		/// @param assetType The asset type to register.
		static void RegisterAssetType(const AssetType& assetType);

		/// @brief The type's name.
		std::string name;
		/// @brief The file extension(s) associated with this asset type.
		std::set<std::string> importExtensions;
		/// @brief The asset's constructor.
		Constructor constructor;
	private:
		static std::vector<AssetType> assetTypes;
	};

/// @brief A macro to register an asset type.
/// @param type The asset type to register.
/// @param extensions A vector containing the file extension(s) associated with this asset type.
#define WFE_ASSET_TYPE(type, extensions) \
struct AssetType##type##Constructor { \
	static wfe::Asset* CreateAsset(wfe::Program* program, wfe::uint64_t id) { \
		return dynamic_cast<wfe::Asset*>(new type(program, id)); \
	} \
	AssetType##type##Constructor() { \
		wfe::AssetType assetType; \
		assetType.name = wfe::GetTypeName<type>(); \
		assetType.importExtensions = extensions; \
		assetType.constructor = CreateAsset; \
		try { \
			wfe::AssetType::RegisterAssetType(assetType); \
		} catch (const std::invalid_argument&) { } \
	} \
}; \
static inline AssetType##type##Constructor assetType##type##ConstructorInstance{};
}