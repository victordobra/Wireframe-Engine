#include "Asset.hpp"
#include "AssetManager.hpp"

namespace wfe {
	// Public functions
	Asset::Asset(Program* program, uint64_t id, const std::filesystem::path& path) : program(program), id(id), path(path.lexically_normal()) {
		// Lock the asset manager mutex
		AssetManager* assetManager = program->GetAssetManager();

		uint32_t locked = 0;
		while(!assetManager->assetsMutex.compare_exchange_strong(locked, 1))
			locked = 0;
		
		// Add the ID to the asset manager map, if valid
		if(id != UINT64_T_MAX)
			assetManager->assetsID.insert({ id, this });

		// Add the path to the asset manager map, if valid
		if(!this->path.empty())
			assetManager->assetsPath.insert({ this->path, this });

		// Unlock the asset manager mutex
		assetManager->assetsMutex = 0;
	}

	void Asset::SetID(uint64_t newID) {
		// Lock the asset manager mutex
		AssetManager* assetManager = program->GetAssetManager();

		uint32_t locked = 0;
		while(!assetManager->assetsMutex.compare_exchange_strong(locked, 1))
			locked = 0;

		// Remove the old ID from the asset manager map, if valid
		if(id != UINT64_T_MAX)
			assetManager->assetsID.erase(id);
		
		// Set the new ID and add it to the asset manager map, if valid
		id = newID;
		if(id != UINT64_T_MAX)
			assetManager->assetsID.insert({ id, this });

		// Unlock the asset manager mutex
		assetManager->assetsMutex = 0;
	}
	void Asset::SetPath(const std::filesystem::path& newPath) {
		// Lock the asset manager mutex
		AssetManager* assetManager = program->GetAssetManager();

		uint32_t locked = 0;
		while(!assetManager->assetsMutex.compare_exchange_strong(locked, 1))
			locked = 0;

		// Remove the old path from the asset manager map, if valid
		if(!path.empty())
			assetManager->assetsPath.erase(path);
		
		// Set the new path and add it to the asset manager map, if valid
		path = newPath.lexically_normal();
		if(!path.empty())
			assetManager->assetsPath.insert({ path, this });

		// Unlock the asset manager mutex
		assetManager->assetsMutex = 0;
	}

	Asset::~Asset() {
		// Lock the asset manager mutex
		AssetManager* assetManager = program->GetAssetManager();

		uint32_t locked = 0;
		while(!assetManager->assetsMutex.compare_exchange_strong(locked, 1))
			locked = 0;

		// Remove the old ID from the asset manager map, if valid
		if(id != UINT64_T_MAX)
			assetManager->assetsID.erase(id);

		// Remove the old path from the asset manager map, if valid
		if(!path.empty())
			assetManager->assetsPath.erase(path);

		// Unlock the asset manager mutex
		assetManager->assetsMutex = 0;
	}
}