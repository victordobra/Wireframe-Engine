#include "Asset.h"
#include <fstream>
#include <vector>

namespace mge {
	Asset::Asset(const char* fileLocation, AssetLoadResult& result) {
		std::ifstream fileInput(fileLocation, std::ios::binary);
		if (!fileInput) {
			result = AssetLoadResult::FILE_NOT_FOUND;
			return;
		}

		fileInput.read((char*)this, sizeof(*this));
		fileInput.close();

		if (!fileInput.good()) {
			result = AssetLoadResult::OTHER;
			return;
		}

		result = AssetLoadResult::SUCCESS;
	}
	AssetSaveResult Asset::SaveToFile(const char* fileLocation) const {
		std::ofstream fileOutput(fileLocation);
		if (!fileOutput)
			return AssetSaveResult::FILE_NOT_FOUND;

		fileOutput.write((char*)this, sizeof(*this));
		fileOutput.close();

		if (!fileOutput.good())
			return AssetSaveResult::OTHER;

		return AssetSaveResult::SUCCESS;
	}


	const char* FromAssetLoadResultToString(AssetLoadResult loadResult) {
		switch (loadResult) {
		case AssetLoadResult::SUCCESS:
			return "SUCCESS";
		case AssetLoadResult::FILE_CORRUPTED:
			return "FILE_CORRUPTED";
		case AssetLoadResult::FILE_NOT_FOUND:
			return "FILE_NOT_FOUND";
		case AssetLoadResult::OTHER:
			return "OTHER";
		default:
			return "Unknown error";
		}
	}
	const char* FromAssetSaveResultToString(AssetSaveResult saveResult) {
		switch (saveResult) {
		case AssetSaveResult::SUCCESS:
			return "SUCCESS";
		case AssetSaveResult::FILE_NOT_FOUND:
			return "FILE_NOT_FOUND";
		case AssetSaveResult::OTHER:
			return "OTHER";
		default:
			return "Unknown error";
		}
	}
}