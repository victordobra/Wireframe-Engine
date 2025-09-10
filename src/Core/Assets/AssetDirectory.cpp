#include "AssetDirectory.hpp"
#include "Core/Parsers/Utils/JSONParser.hpp"
#include <future>
#include <utility>

namespace wfe {
	// Asset jobs
	static Asset* LoadAssetJob(Program* program, const AssetType& assetType, uint64_t id, const std::filesystem::path& path) {
		// Create and load the asset
		Asset* asset = assetType.constructor(program, id, path);
		asset->Load();

		return asset;
	}
	static void SaveAssetJob(const Asset* asset) {
		// Save the asset to the file
		asset->Save();
	}
	static Asset* ImportAssetJob(Program* program, const AssetType& assetType, uint64_t id, const std::filesystem::path& path) {
		// Create and import the asset
		Asset* asset = assetType.constructor(program, id, path);
		asset->Import();

		return asset;
	}
	static void ExportAssetJob(const Asset* asset) {
		// Export the asset to the file
		asset->Export();
	}

	// Internal helper funoctions
	void AssetDirectory::GetLoadInterval(size_t index, const std::unordered_map<uint64_t, size_t>& indices, const std::vector<std::vector<uint64_t>>& dependencies, std::vector<std::pair<size_t, size_t>>& loadIntervals) {
		// Get the asset's dependencies
		const std::vector<uint64_t>& assetDependencies = dependencies[index];

		// Check if the asset was already used
		if(loadIntervals[index].first != SIZE_T_MAX) {
#if defined(WFE_BUILD_MODE_DEBUG)
			// Check if its height is greater than the heights of all children
			for(uint64_t childID : assetDependencies) {
				// Get the child's index
				auto childIter = indices.find(childID);
				if(childIter == indices.end())
					continue;
				size_t childIndex = childIter->second;

				if(loadIntervals[childIndex].first >= loadIntervals[index].first)
					throw std::invalid_argument("Detected circular dependency in asset directory!");
			}
#endif

			return;
		}

		// Set all children intervals
		loadIntervals[index].first = 0;
		for(uint64_t childID : assetDependencies) {
			// Get the child's index
			auto childIter = indices.find(childID);
			if(childIter == indices.end())
				continue;
			size_t childIndex = childIter->second;

			// Get the child's load interval
			GetLoadInterval(childIndex, indices, dependencies, loadIntervals);

			// Update the interval start, if the current asset needs more time to load
			if(loadIntervals[childIndex].first >= loadIntervals[index].first)
				loadIntervals[index].first = loadIntervals[childIndex].first + 1;
		}

		// Update the load intervals of all children
		for(uint64_t childID : assetDependencies) {
			// Get the child's index
			auto childIter = indices.find(childID);
			if(childIter == indices.end())
				continue;
			size_t childIndex = childIter->second;

			// Update the child's load interval
			if(loadIntervals[childIndex].second >= loadIntervals[index].first)
				loadIntervals[childIndex].second = loadIntervals[index].first - 1;
		}
	}
	void AssetDirectory::GetAssetLoadOrder(const std::vector<uint64_t>& ids, const std::vector<std::vector<uint64_t>>& dependencies, std::vector<std::vector<size_t>>& loadStartOrder, std::vector<std::vector<size_t>>& loadEndOrder) {
		// Assign indices to all assets
		std::unordered_map<uint64_t, size_t> indices;
		for(size_t i = 0; i != ids.size(); ++i)
			indices.insert({ ids[i], i });
		
		// Calculate all load intervals
		std::vector<std::pair<size_t, size_t>> loadIntervals(ids.size(), { SIZE_T_MAX, SIZE_T_MAX });
		for(size_t i = 0; i != ids.size(); ++i)
			GetLoadInterval(i, indices, dependencies, loadIntervals);
		
		// Get the maximum load interval
		size_t maxInterval = 0;
		for(size_t i = 0; i != ids.size(); ++i) {
			if(loadIntervals[i].first > maxInterval)
				maxInterval = loadIntervals[i].first;
		}
		
		// Create the load orders
		loadStartOrder.resize(maxInterval + 1);
		loadEndOrder.resize(maxInterval + 1);

		for(size_t i = 0; i != ids.size(); ++i) {
			// Get the asset's load interval
			size_t startInterval = loadIntervals[i].first;
			size_t endInterval = loadIntervals[i].second;

			// Update the end interval, if required
			if(endInterval == SIZE_T_MAX)
				endInterval = startInterval;

			// Add the asset to the load orders
			loadStartOrder[startInterval].push_back(i);
			loadEndOrder[endInterval].push_back(i);
		}
	}

	// Public functions
	void AssetDirectory::Load() {
		// Destroy the old assets
		for(size_t i = 0; i != assets.size(); ++i)
			delete assets[i];
		assets.clear();

		// Open the directory's info file
		std::filesystem::path infoFilePath = (path / "assets.bin").lexically_normal();
		std::ifstream stream(infoFilePath, std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open asset directory info file \"" + infoFilePath.string() + "\" for reading!");
		
		// Read the asset count
		uint64_t assetCount = BinaryReadUint64BE(stream);

		// Read every asset
		assets.resize(assetCount);

		std::vector<uint64_t> ids(assetCount);
		std::vector<std::filesystem::path> paths(assetCount);
		std::vector<const AssetType*> assetTypes(assetCount);
		std::vector<std::vector<uint64_t>> dependencies(assetCount);

		for(size_t i = 0; i != assetCount; ++i) {
			// Read the asset's ID
			ids[i] = BinaryReadUint64BE(stream);

			// Read the asset's path
			uint64_t pathLength = BinaryReadUint64BE(stream);
			std::string relativePath(pathLength, 0);
			stream.read(relativePath.data(), pathLength);
			paths[i] = (path / relativePath).lexically_normal();

			// Read the asset's type
			uint64_t typeNameLength = BinaryReadUint64BE(stream);
			std::string typeName(typeNameLength, 0);
			stream.read(typeName.data(), typeNameLength);

			assetTypes[i] = &AssetType::GetAssetTypeName(typeName);
			if(assetTypes[i] == nullptr)
				throw std::runtime_error("Failed to find asset type \"" + typeName + "\"!");

			// Read the asset's dependencies
			uint64_t dependencyCount = BinaryReadUint64BE(stream);
			dependencies[i].resize(dependencyCount);

			for(size_t j = 0; j != dependencyCount; ++j)
				dependencies[i][j] = BinaryReadUint64BE(stream);
		}

		stream.close();

		// Get the asset load order
		std::vector<std::vector<size_t>> loadStartOrder, loadEndOrder;
		GetAssetLoadOrder(ids, dependencies, loadStartOrder, loadEndOrder);

		// Load the assets in the correct order
		std::vector<std::future<Asset*>> loadFutures(assetCount);
		
		for(size_t i = 0; i != loadStartOrder.size(); ++i) {
			for(size_t j = 0; j != loadStartOrder[i].size(); ++j) {
				// Get the asset's index
				size_t index = loadStartOrder[i][j];

				// Load the asset
				loadFutures[index] = std::async(std::launch::async | std::launch::deferred, LoadAssetJob, program, *assetTypes[index], ids[index], paths[index]);
			}

			for(size_t j = 0; j != loadEndOrder[i].size(); ++j) {
				// Get the asset's index
				size_t index = loadEndOrder[i][j];

				// Wait for the asset to finish loading
				assets[index] = loadFutures[index].get();
			}
		}
	}
	void AssetDirectory::Save(bool saveAssets) const {
		// Store all asset save futures, if required
		std::vector<std::future<void>> saveFutures;

		if(saveAssets) {
			// Save all assets in the directory
			saveFutures.resize(assets.size());
			for(size_t i = 0; i != assets.size(); ++i)
				saveFutures[i] = std::async(std::launch::async | std::launch::deferred, SaveAssetJob, assets[i]);
		}

		// Open the directory's info file
		std::filesystem::path infoFilePath = (path / "assets.bin").lexically_normal();
		std::ofstream stream(infoFilePath, std::ios::binary);
		if(!stream) {
			// Wait for all assets to finish saving
			for(size_t i = 0; i != saveFutures.size(); ++i)
				saveFutures[i].wait();

			throw std::runtime_error("Failed to open asset directory info file \"" + infoFilePath.string() + "\" for writing!");
		}
		
		// Write the asset count
		BinaryWriteUint64BE(stream, assets.size());

		// Write every asset
		for(size_t i = 0; i != assets.size(); ++i) {
			// Write the asset's ID
			BinaryWriteUint64BE(stream, assets[i]->GetID());

			// Write the asset's path
			std::string relativePath = assets[i]->GetPath().lexically_relative(path).string();
			BinaryWriteUint64BE(stream, relativePath.size());
			stream.write(relativePath.data(), relativePath.size());

			// Write the asset's type
			std::string typeName = AssetType::GetAssetType(assets[i]).name;
			BinaryWriteUint64BE(stream, typeName.size());
			stream.write(typeName.data(), typeName.size());

			// Write the asset's dependencies
			std::vector<Asset*> assetDependencies = assets[i]->GetDependencies();
			BinaryWriteUint64BE(stream, assetDependencies.size());
			
			for(size_t j = 0; j != assetDependencies.size(); ++j)
				BinaryWriteUint64BE(stream, assetDependencies[j]->GetID());
		}

		stream.close();

		if(saveAssets) {
			// Wait for all assets to finish saving
			for(size_t i = 0; i != saveFutures.size(); ++i)
				saveFutures[i].wait();
		}
	}
	void AssetDirectory::Import() {
		// Destroy the old assets
		for(size_t i = 0; i != assets.size(); ++i)
			delete assets[i];
		assets.clear();

		// Open the directory's info file
		std::filesystem::path infoFilePath = (path / "assets.json").lexically_normal();
		std::ifstream stream(infoFilePath);
		if(!stream)
			throw std::runtime_error("Failed to open asset directory info file \"" + infoFilePath.string() + "\" for reading!");
		
		// Parse the JSON asset file
		JSONObject jsonObject;
		if(!jsonObject.ReadObject(stream))
			throw std::runtime_error("Failed to read asset directory info file \"" + infoFilePath.string() + "\"!");

		stream.close();

		// Get the asset count
		const JSONArray& assetArray = jsonObject.GetJSONArrayValue("assets");
		size_t assetCount = assetArray.GetValueCount();

		// Get every asset
		assets.resize(assetCount);

		std::vector<uint64_t> ids(assetCount);
		std::vector<std::filesystem::path> paths(assetCount);
		std::vector<const AssetType*> assetTypes(assetCount);
		std::vector<std::vector<uint64_t>> dependencies(assetCount);

		for(size_t i = 0; i != assetCount; ++i) {
			// Get the asset's object
			const JSONObject& assetObject = assetArray.GetJSONObjectValue(i);

			// Get the asset's ID
			ids[i] = (uint64_t)assetObject.GetIntValue("id");

			// Get the asset's path
			paths[i] = (path / assetObject.GetStringValue("path")).lexically_normal();

			if(assetObject.HasValue("type")) {
				// Get the asset's type
				assetTypes[i] = &AssetType::GetAssetTypeName(assetObject.GetStringValue("type"));
			} else {
				// Deduce the asset's type from its file extension
				std::string extension = paths[i].extension().string();
				assetTypes[i] = &AssetType::GetAssetTypeExtension(extension);
			}
	
			// Get the asset's dependencies
			const JSONArray& dependenciesArray = assetObject.GetJSONArrayValue("dependencies");

			dependencies[i].resize(dependenciesArray.GetValueCount());
			for(size_t j = 0; j != dependencies[i].size(); ++j)
				dependencies[i][j] = (uint64_t)dependenciesArray.GetIntValue(j);
		}

		// Get the asset load order
		std::vector<std::vector<size_t>> loadStartOrder, loadEndOrder;
		GetAssetLoadOrder(ids, dependencies, loadStartOrder, loadEndOrder);

		// Load the assets in the correct order
		std::vector<std::future<Asset*>> loadFutures(assetCount);
		
		for(size_t i = 0; i != loadStartOrder.size(); ++i) {
			for(size_t j = 0; j != loadStartOrder[i].size(); ++j) {
				// Get the asset's index
				size_t index = loadStartOrder[i][j];

				// Load the asset
				loadFutures[index] = std::async(std::launch::async | std::launch::deferred, ImportAssetJob, program, *assetTypes[index], ids[index], paths[index]);
			}

			for(size_t j = 0; j != loadEndOrder[i].size(); ++j) {
				// Get the asset's index
				size_t index = loadEndOrder[i][j];

				// Wait for the asset to finish loading
				assets[index] = loadFutures[index].get();
			}
		}
	}
	void AssetDirectory::Export(bool exportAssets) const {
		// Store all asset export futures, if required
		std::vector<std::future<void>> exportFutures;

		if(exportAssets) {
			// Export all assets in the directory
			exportFutures.resize(assets.size());
			for(size_t i = 0; i != assets.size(); ++i)
				exportFutures[i] = std::async(std::launch::async | std::launch::deferred, ExportAssetJob, assets[i]);
		}

		// Generate the directory's info object
		JSONArray assetArray;

		// Generate the asset objects
		for(size_t i = 0; i != assets.size(); ++i) {
			// Create the asset's object
			JSONObject assetObject;

			// Get the asset's dependencies
			std::vector<Asset*> assetDependencies = assets[i]->GetDependencies();

			// Add the ID, path and type values
			assetObject.AddIntValue("id", assets[i]->GetID());
			assetObject.AddStringValue("path", assets[i]->GetPath().lexically_relative(path).string());
			assetObject.AddStringValue("type", AssetType::GetAssetType(assets[i]).name);

			// Create the dependencies array
			JSONArray dependenciesArray;
			for(size_t j = 0; j != assetDependencies.size(); ++j) {
				if(assetDependencies[j]->GetID() != UINT64_T_MAX)
					dependenciesArray.AddIntValue(assetDependencies[j]->GetID());
			}
			assetObject.AddJSONArrayValue("dependencies", std::move(dependenciesArray));

			// Add the asset object to the array
			assetArray.AddJSONObjectValue(std::move(assetObject));
		}

		// Create the total JSON object
		JSONObject jsonObject;
		jsonObject.AddJSONArrayValue("assets", std::move(assetArray));

		// Open the directory's info file
		std::filesystem::path infoFilePath = (path / "assets.json").lexically_normal();
		std::ofstream stream(infoFilePath);
		if(!stream) {
			// Wait for all assets to finish saving
			for(size_t i = 0; i != exportFutures.size(); ++i)
				exportFutures[i].wait();

			throw std::runtime_error("Failed to open asset directory info file \"" + infoFilePath.string() + "\" for writing!");
		}
		
		// Write the JSON object
		jsonObject.WriteObject(stream);

		stream.close();

		if(exportAssets) {
			// Wait for all assets to finish exporting
			for(size_t i = 0; i != exportFutures.size(); ++i)
				exportFutures[i].wait();
		}
	}

	void AssetDirectory::SetPath(const std::filesystem::path& newPath, bool updateAssetPaths) {
		if(updateAssetPaths) {
			// Update all asset paths
			for(size_t i = 0; i != assets.size(); ++i) {
				// Get the asset's relative path
				std::filesystem::path relativePath = assets[i]->GetPath().lexically_relative(path);

				// Update the asset's path, now relative to the directory's current paath
				assets[i]->SetPath((newPath / relativePath).lexically_normal());
			}
		}

		// Set the asset's path
		path = newPath.lexically_normal();
	}

	AssetDirectory::~AssetDirectory() {
		// Destroy the assets
		for(size_t i = 0; i != assets.size(); ++i)
			delete assets[i];
	}
}