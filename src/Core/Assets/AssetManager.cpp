#include "AssetManager.hpp"
#include "Asset.hpp"
#include "Core/Parsers/WFEONParser.hpp"
#include <fstream>

namespace wfe {
	// Asset jobs
	static Asset* LoadAssetJob(Program* program, const AssetType& assetType, uint64_t id, const std::string& path) {
		// Create the asset
		Asset* asset = assetType.constructor(program, id);

		// Load the asset from the file
		std::ifstream stream(path, std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open asset file \"" + path + "\" for reading!");

		asset->Load(stream);
		stream.close();

		return asset;
	}
	static void SaveAssetJob(const Asset* asset, const std::string& path) {
		// Save the asset to the file
		std::ofstream stream(path, std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open asset file \"" + path + "\" for writing!");
		
		asset->Save(stream);
		stream.close();
	}
	static Asset* ImportAssetJob(Program* program, const AssetType& assetType, uint64_t id, const std::string& path) {
		// Create the asset
		Asset* asset = assetType.constructor(program, id);

		// Import the asset from the file
		asset->Import(path);

		return asset;
	}
	static void ExportAssetJob(const Asset* asset, const std::string& path) {
		// Export the asset to the file
		asset->Export(path);
	}

	// Internal helper funoctions
	void AssetManager::GetLoadInterval(size_t index, const std::unordered_map<uint64_t, size_t>& indices, const std::vector<std::vector<uint64_t>>& dependencies, std::vector<std::pair<size_t, size_t>>& loadIntervals) {
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
		size_t intervalStart = 0;
		for(uint64_t childID : assetDependencies) {
			// Get the child's index
			auto childIter = indices.find(childID);
			if(childIter == indices.end())
				continue;
			size_t childIndex = childIter->second;

			// Get the child's load interval
			GetLoadInterval(childIndex, indices, dependencies, loadIntervals);

			// Update the interval start, if the current asset needs more time to load
			if(loadIntervals[childIndex].first >= intervalStart)
				intervalStart = loadIntervals[childIndex].first + 1;
		}

		// Set the asset's load interval
		loadIntervals[index] = { intervalStart, SIZE_T_MAX };

		// Update the load intervals of all children
		for(uint64_t childID : assetDependencies) {
			// Get the child's index
			auto childIter = indices.find(childID);
			if(childIter == indices.end())
				continue;
			size_t childIndex = childIter->second;

			// Update the child's load interval
			if(loadIntervals[childIndex].second >= intervalStart)
				loadIntervals[childIndex].second = intervalStart - 1;
		}
	}
	void AssetManager::GetAssetLoadOrder(const std::vector<uint64_t>& ids, const std::vector<std::vector<uint64_t>>& dependencies, std::vector<std::vector<size_t>>& loadStartOrder, std::vector<std::vector<size_t>>& loadEndOrder) {
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
	void AssetManager::LoadDirectory(const std::string& path) {
		// Open the directory's info file
		std::ifstream stream(path + ".assets", std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open asset directory info file \"" + path + ".assets\" for reading!");
		
		// Read the asset count
		uint64_t assetCount;
		stream.read((char*)assetCount, sizeof(uint64_t));

		// Read every asset
		Directory directory;
		directory.paths.resize(assetCount);
		directory.assets.resize(assetCount);

		std::vector<uint64_t> ids(assetCount);
		std::vector<const AssetType*> assetTypes(assetCount);
		std::vector<std::vector<uint64_t>> dependencies(assetCount);

		for(size_t i = 0; i != assetCount; ++i) {
			// Read the asset's path
			uint64_t pathLength;
			stream.read((char*)&pathLength, sizeof(uint64_t));

			directory.paths[i].resize(pathLength, '\0');
			stream.read((char*)directory.paths[i].data(), pathLength);

			// Read the asset's ID
			stream.read((char*)&ids[i], sizeof(uint64_t));

			// Read the asset's type
			uint64_t typeNameLength;
			stream.read((char*)&typeNameLength, sizeof(uint64_t));

			std::string typeName(typeNameLength, '\0');
			stream.read((char*)typeName.data(), typeNameLength);

			assetTypes[i] = &AssetType::GetAssetTypeName(typeName);
			if(assetTypes[i] == nullptr)
				throw std::runtime_error("Failed to find asset type \"" + typeName + "\"!");

			// Read the asset's dependencies
			uint64_t dependencyCount;
			stream.read((char*)&dependencyCount, sizeof(uint64_t));
			dependencies[i].resize(dependencyCount);

			for(size_t j = 0; j != dependencyCount; ++j)
				stream.read((char*)&dependencies[i][j], sizeof(uint64_t));
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
				loadFutures[index] = std::async(std::launch::async | std::launch::deferred, LoadAssetJob, program, *assetTypes[index], ids[index], path + directory.paths[index]);
			}

			for(size_t j = 0; j != loadEndOrder[i].size(); ++j) {
				// Get the asset's index
				size_t index = loadEndOrder[i][j];

				// Wait for the asset to finish loading
				directory.assets[index] = loadFutures[index].get();
				
				// Add the asset to the map
				assets.insert({ ids[index], directory.assets[index] });
			}
		}

		// Add the directory to the asset manager
		directories.insert({ path, directory });
	}
	void AssetManager::SaveDirectory(const std::string& path, bool saveAssets) const {
		// Identify the directory
		const auto directoryIter = directories.find(path);
		if(directoryIter == directories.end())
			throw std::runtime_error("Failed to find loaded asset directory \"" + path + "\"!");
		const Directory& directory = directoryIter->second;

		std::vector<std::future<void>> saveFutures;

		if(saveAssets) {
			// Save all assets in the directory
			saveFutures.resize(directory.assets.size());
			for(size_t i = 0; i != directory.assets.size(); ++i)
				saveFutures[i] = std::async(std::launch::async | std::launch::deferred, SaveAssetJob, directory.assets[i], path + directory.paths[i]);
		}

		// Open the directory's info file
		std::ofstream stream(path + ".assets", std::ios::binary);
		if(!stream) {
			// Wait for all assets to finish saving
			for(size_t i = 0; i != saveFutures.size(); ++i)
				saveFutures[i].wait();

			throw std::runtime_error("Failed to open asset directory info file \"" + path + ".assets\" for writing!");
		}
		
		// Write the asset count
		uint64_t assetCount = directory.assets.size();
		stream.write((char*)&assetCount, sizeof(uint64_t));

		// Write every asset
		for(size_t i = 0; i != directory.assets.size(); ++i) {
			// Write the asset's path
			uint64_t pathLength = directory.paths[i].size();
			stream.write((char*)&pathLength, sizeof(uint64_t));
			stream.write((char*)directory.paths[i].data(), pathLength);

			// Write the asset's ID
			uint64_t id = directory.assets[i]->GetID();
			stream.write((char*)&id, sizeof(uint64_t));

			// Write the asset's type
			std::string typeName = WFE_TYPE_NAME(*(directory.assets[i]));
			uint64_t typeNameLength = typeName.size();
			stream.write((char*)&typeNameLength, sizeof(uint64_t));
			stream.write((char*)typeName.data(), typeNameLength);

			// Write the asset's dependencies
			std::vector<Asset*> assetDependencies = directory.assets[i]->GetDependencies();
			std::vector<uint64_t> dependencyIDs(assetDependencies.size());
			for(size_t j = 0; j != assetDependencies.size(); ++j)
				dependencyIDs[j] = assetDependencies[j]->GetID();

			uint64_t dependencyCount = assetDependencies.size();
			stream.write((char*)&dependencyCount, sizeof(uint64_t));
			
			for(size_t j = 0; j != dependencyCount; ++j)
				stream.write((char*)&dependencyIDs[j], sizeof(uint64_t));
		}

		stream.close();

		if(saveAssets) {
			// Wait for all assets to finish saving
			for(size_t i = 0; i != saveFutures.size(); ++i)
				saveFutures[i].wait();
		}
	}
	void AssetManager::ImportDirectory(const std::string& path) {
		// Open the directory's info file
		std::ifstream stream(path + ".assets", std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open asset directory info file \"" + path + ".assets\" for reading!");
		
		// Parse the WFEON file
		WFEONObject wfeonObject;
		wfeonObject.Parse(stream);

		stream.close();

		// Get the asset count
		const WFEONObject::WFEONValue& assetValue = wfeonObject.values.at("assets");
		if(assetValue.type != WFEONObject::VALUE_TYPE_OBJECT)
			throw std::runtime_error("Failed to load asset directory \"" + path + "\": \"assets\" value is not an object!");
		size_t assetCount = assetValue.count;

		// Get every asset
		Directory directory;
		directory.paths.resize(assetCount);
		directory.assets.resize(assetCount);

		std::vector<uint64_t> ids(assetCount);
		std::vector<const AssetType*> assetTypes(assetCount);
		std::vector<std::vector<uint64_t>> dependencies(assetCount);

		for(size_t i = 0; i != assetCount; ++i) {
			// Get the asset's object
			const WFEONObject& assetObject = wfeonObject.objectValues[assetValue.startIndex + i];

			// Get the asset's path
			const WFEONObject::WFEONValue& pathValue = assetObject.values.at("path");
			if(pathValue.type != WFEONObject::VALUE_TYPE_STRING)
				throw std::runtime_error("Failed to load asset directory \"" + path + "\": \"path\" value is not a string!");
			if(pathValue.count != 1)
				throw std::runtime_error("Failed to load asset directory \"" + path + "\": \"path\" value is not a single string!");
			directory.paths[i] = assetObject.stringValues[pathValue.startIndex];

			// Get the asset's ID
			const WFEONObject::WFEONValue& idValue = assetObject.values.at("id");
			if(idValue.type != WFEONObject::VALUE_TYPE_INT)
				throw std::runtime_error("Failed to load asset directory \"" + path + "\": \"id\" value is not an integer!");
			if(idValue.count != 1)
				throw std::runtime_error("Failed to load asset directory \"" + path + "\": \"id\" value is not a single integer!");
			ids[i] = (uint64_t)assetObject.intValues[idValue.startIndex];

			// Get the file extension
			size_t fileStart = directory.paths[i].find_last_of("/\\");
			if(fileStart == std::string::npos) {
				fileStart = 0;
			} else {
				++fileStart;
			}
			size_t extensionStart = directory.paths[i].find_last_of('.');
			if(extensionStart == std::string::npos || extensionStart < fileStart)
				throw std::runtime_error("Failed to load asset directory \"" + path + "\": \"path\" value does not contain a file extension!");
			
			std::string extension = directory.paths[i].substr(extensionStart + 1);

			// Get the asset's type
			assetTypes[i] = &AssetType::GetAssetTypeExtension(extension);
	
			// Get the asset's dependencies
			const WFEONObject::WFEONValue& dependenciesValue = assetObject.values.at("dependencies");
			if(dependenciesValue.type != WFEONObject::VALUE_TYPE_COUNT) {
				if(dependenciesValue.type != WFEONObject::VALUE_TYPE_INT)
					throw std::runtime_error("Failed to load asset directory \"" + path + "\": \"dependencies\" value is not an object!");
				if(dependenciesValue.count != 1)
					throw std::runtime_error("Failed to load asset directory \"" + path + "\": \"dependencies\" value is not a single object!");
				
				dependencies[i].resize(dependenciesValue.count);
				for(size_t j = 0; j != dependenciesValue.count; ++j)
					dependencies[i][j] = (uint64_t)assetObject.intValues[dependenciesValue.startIndex + j];
			}
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

				// Import the asset
				loadFutures[index] = std::async(std::launch::async | std::launch::deferred, ImportAssetJob, program, *assetTypes[index], ids[index], path + directory.paths[index]);
			}

			for(size_t j = 0; j != loadEndOrder[i].size(); ++j) {
				// Get the asset's index
				size_t index = loadEndOrder[i][j];

				// Wait for the asset to finish importing
				directory.assets[index] = loadFutures[index].get();
				
				// Add the asset to the map
				assets.insert({ ids[index], directory.assets[index] });
			}
		}

		// Add the directory to the asset manager
		directories.insert({ path, directory });

	}
	void AssetManager::ExportDirectory(const std::string& path, bool exportAssets) const {
		// Identify the directory
		const auto directoryIter = directories.find(path);
		if(directoryIter == directories.end())
			throw std::runtime_error("Failed to find loaded asset directory \"" + path + "\"!");
		const Directory& directory = directoryIter->second;

		std::vector<std::future<void>> exportFutures;

		if(exportAssets) {
			// Export all assets in the directory
			exportFutures.resize(directory.assets.size());
			for(size_t i = 0; i != directory.assets.size(); ++i)
				exportFutures[i] = std::async(std::launch::async | std::launch::deferred, ExportAssetJob, directory.assets[i], path + directory.paths[i]);
		}

		// Generate the directory's info object
		WFEONObject wfeonObject;
		wfeonObject.values.insert({ "assets", { WFEONObject::VALUE_TYPE_OBJECT, (uint64_t)directory.assets.size(), 0 } });
		wfeonObject.objectValues.resize(directory.assets.size());

		// Generate the asset objects
		for(size_t i = 0; i != directory.assets.size(); ++i) {
			// Get the asset's object
			WFEONObject& assetObject = wfeonObject.objectValues[i];

			// Get the asset's dependencies
			std::vector<Asset*> assetDependencies = directory.assets[i]->GetDependencies();

			// Set all value vector counts
			assetObject.intValues.resize(1 + assetDependencies.size());
			assetObject.stringValues.resize(2);

			// Create the path value
			assetObject.values.insert({ "path", { WFEONObject::VALUE_TYPE_STRING, 1, 0 } });
			assetObject.stringValues[0] = directory.paths[i];

			// Create the ID value
			assetObject.values.insert({ "id", { WFEONObject::VALUE_TYPE_INT, 1, 0 } });
			assetObject.intValues[0] = (int64_t)directory.assets[i]->GetID();

			// Create the dependencies value
			assetObject.values.insert({ "dependencies", { WFEONObject::VALUE_TYPE_INT, (uint64_t)assetDependencies.size(), 1 } });
			for(size_t j = 0; j != assetDependencies.size(); ++j)
				assetObject.intValues[j + 1] = (int64_t)assetDependencies[j]->GetID();
		}

		// Open the directory's info file
		std::ofstream stream(path + ".assets", std::ios::binary);
		if(!stream) {
			// Wait for all assets to finish saving
			for(size_t i = 0; i != exportFutures.size(); ++i)
				exportFutures[i].wait();

			throw std::runtime_error("Failed to open asset directory info file \"" + path + ".assets\" for writing!");
		}
		
		// Write the WFEON object
		wfeonObject.Write(stream);

		stream.close();

		if(exportAssets) {
			// Wait for all assets to finish exporting
			for(size_t i = 0; i != exportFutures.size(); ++i)
				exportFutures[i].wait();
		}
	}
	void AssetManager::UnloadDirectory(const std::string& path) {
		// Get the directory iterator
		auto directoryIter = directories.find(path);
		if(directoryIter == directories.end())
			throw std::runtime_error("Failed to find loaded asset directory \"" + path + "\"!");
		Directory& directory = directoryIter->second;

		// Destroy all assets in the directory
		for(size_t i = 0; i != directory.assets.size(); ++i) {
			// Remove the asset from the map and destroy it
			assets.erase(directory.assets[i]->GetID());
			delete directory.assets[i];
		}

		// Remove the directory from the asset manager
		directories.erase(directoryIter);
	}
}