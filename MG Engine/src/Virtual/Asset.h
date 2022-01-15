#pragma once
#include <string>

namespace mge {
	enum class AssetLoadResult {
		SUCCESS,
		FILE_NOT_FOUND,
		FILE_CORRUPTED,
		OTHER
	};
	enum class AssetSaveResult {
		SUCCESS,
		FILE_NOT_FOUND,
		OTHER
	};

	class Asset {
	public:
		Asset() = default;
		Asset(const Asset&) = default;
		Asset(Asset&&) noexcept = default;
		Asset(const char* fileLocation, AssetLoadResult& result);

		virtual AssetSaveResult SaveToFile(const char* fileLocation) const;

		~Asset() = default;
	};

	const char* FromAssetLoadResultToString(AssetLoadResult loadResult);
	const char* FromAssetSaveResultToString(AssetSaveResult saveResult);
}