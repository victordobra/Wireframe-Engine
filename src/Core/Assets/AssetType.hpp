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
		/// @brief The maximum possible total number of asset types
		static inline const size_t MAX_ASSET_TYPE_COUNT = 128;
		/// @brief The maximum length of an asset type, including the null termination character.
		static inline const size_t MAX_TYPE_NAME_LENGTH = 64;
		/// @brief The maximum number of import file extensions for the asset.
		static inline const size_t MAX_EXTENSION_COUNT = 8;
		/// @brief The maximum length of an import file extension, including the null termination character.
		static inline const size_t MAX_EXTENSION_LENGTH = 8;

		/// @brief The asset constructor function type.
		typedef Asset*(*Constructor)(Program* program, uint64_t id);

		/// @brief Gets the number of registered asset types.
		/// @return The number of registered asset types.
		static const size_t GetAssetTypeCount() {
			return assetTypeCount;
		}
		/// @brief Gets all registered asset types.
		/// @return A pointer to the array of all registered asset types.
		static const AssetType* GetAssetTypes() {
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

		/// @brief Creates a default asset type.
		constexpr AssetType() = default;
		/// @brief Copies the given asset type.
		/// @param other The asset type to copy.
		constexpr AssetType(const AssetType& other) = default;
		/// @brief Moves the given asset type.
		/// @param other The asset type to move.
		constexpr AssetType(AssetType&& other) noexcept = default;

		/// @brief Copies the given asset type's info into this asset type.
		/// @param other The asset type whose info to copy.
		/// @return A reference to this asset type.
		AssetType& operator=(const AssetType& other) = default;
		/// @brief Moves the given asset type's info into this asset type.
		/// @param other The asset type whose info to move.
		/// @return A reference to this asset type.
		AssetType& operator=(AssetType&& other) noexcept = default;

		/// @brief Destroys the asset type.
		~AssetType() = default;

		/// @brief The type's name.
		char name[MAX_TYPE_NAME_LENGTH] { };
		/// @brief The number of file extensions associated with this asset type.
		size_t importExtensionCount = 0;
		/// @brief The file extensions associated with this asset type.
		char importExtensions[MAX_EXTENSION_COUNT][MAX_EXTENSION_LENGTH] { };
		/// @brief The asset's constructor.
		Constructor constructor = nullptr;
	private:
		static size_t assetTypeCount;
		static AssetType assetTypes[MAX_ASSET_TYPE_COUNT];
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
		\
		strncpy(assetType.name, wfe::GetTypeName<type>().c_str(), wfe::AssetType::MAX_TYPE_NAME_LENGTH); \
		\
		const char* const EXT_ARRAY[] extensions; \
		assetType.importExtensionCount = sizeof(EXT_ARRAY) / sizeof(const char*); \
		for(wfe::size_t i = 0; i != assetType.importExtensionCount; ++i) \
			strncpy(assetType.importExtensions[i], EXT_ARRAY[i], wfe::AssetType::MAX_EXTENSION_LENGTH); \
		\
		assetType.constructor = CreateAsset; \
		\
		wfe::AssetType::RegisterAssetType(assetType); \
	} \
}; \
static inline AssetType##type##Constructor assetType##type##ConstructorInstance { };
}