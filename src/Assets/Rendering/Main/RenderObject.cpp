#include "RenderObject.hpp"
#include "Core/Utils/BinaryIO.hpp"
#include "Main/Program.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace wfe {
	// Array vertex struct
	struct ArrVertex {
		uint32_t posIndex;
		uint32_t uvIndex;
		uint32_t normIndex;
		uint32_t tanIndex;
		uint32_t bitanIndex;

		bool operator==(const ArrVertex& other) const {
			return posIndex == other.posIndex && uvIndex == other.uvIndex && normIndex == other.normIndex && tanIndex == other.tanIndex && bitanIndex == other.bitanIndex;
		}
	};
}

// Array vertex hash function
template<>
struct std::hash<wfe::ArrVertex> {
	std::size_t operator()(const wfe::ArrVertex& vert) const {
		// Use the boost library hash combine to mix all hashes
		std::hash<uint32_t> hasher;
		size_t res = hasher(vert.posIndex) + 0x9e3779b9;
		res ^= hasher(vert.uvIndex) + 0x9e3779b9 + (res << 6) + (res >> 2);
		res ^= hasher(vert.normIndex) + 0x9e3779b9 + (res << 6) + (res >> 2);
		res ^= hasher(vert.tanIndex) + 0x9e3779b9 + (res << 6) + (res >> 2);
		res ^= hasher(vert.bitanIndex) + 0x9e3779b9 + (res << 6) + (res >> 2);
		
		return res;
	}
};

namespace wfe {
	// Virtual function definitions
	void RenderObject::Load() {
		// Destroy all previous items
		for(size_t i = 0; i != items.size(); ++i)
			delete items[i].mesh;
		items.clear();

		// Open the file stream for reading
		std::ifstream stream(GetPath(), std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open render object file \"" + GetPath().string() + "\" for reading!");

		// Read the material collection's ID
		uint64_t materialCollectionID = BinaryReadUint64(stream);

		if(materialCollectionID != UINT64_T_MAX) {
			// Get the material collection
			materialCollection = dynamic_cast<MaterialCollection*>(GetProgram()->GetAssetManager()->GetAsset(materialCollectionID));
			if(!materialCollection)
				throw std::runtime_error("Invalid material collection ID stored in render object file!");
		} else {
			materialCollection = nullptr;
		}

		// Read the number of items
		uint64_t itemCount = BinaryReadUint64(stream);
		items.resize(itemCount);

		for(size_t i = 0; i != itemCount; ++i) {
			// Read the item's name
			uint64_t nameLength = BinaryReadUint64(stream);
			items[i].name.resize(nameLength);
			stream.read(items[i].name.data(), nameLength);

			// Read the number of vertices
			uint64_t vertexCount = BinaryReadUint64(stream);
			std::vector<RenderMesh::Vertex> vertices(vertexCount);

			// Read all vertices
			for(size_t j = 0; j != vertexCount; ++j) {
				vertices[j].position.x = BinaryReadFloat(stream);
				vertices[j].position.y = BinaryReadFloat(stream);
				vertices[j].position.z = BinaryReadFloat(stream);

				vertices[j].uvCoord.x = BinaryReadFloat(stream);
				vertices[j].uvCoord.y = BinaryReadFloat(stream);

				vertices[j].normal.x = BinaryReadFloat(stream);
				vertices[j].normal.y = BinaryReadFloat(stream);
				vertices[j].normal.z = BinaryReadFloat(stream);

				vertices[j].tangent.x = BinaryReadFloat(stream);
				vertices[j].tangent.y = BinaryReadFloat(stream);
				vertices[j].tangent.z = BinaryReadFloat(stream);

				vertices[j].bitangent.x = BinaryReadFloat(stream);
				vertices[j].bitangent.y = BinaryReadFloat(stream);
				vertices[j].bitangent.z = BinaryReadFloat(stream);
			}

			// Read the number of indices
			uint64_t indexCount = BinaryReadUint64(stream);
			std::vector<uint32_t> indices(indexCount);

			// Read all indices
			for(size_t j = 0; j != indexCount; ++j)
				indices[j] = BinaryReadUint32(stream);

			// Create the mesh
			items[i].mesh = new RenderMesh(GetProgram()->GetRenderer(), vertices, indices);
			
			// Read the material name's length
			uint64_t materialNameLength = BinaryReadUint64(stream);

			items[i].material = nullptr;
			if(materialNameLength) {
				// Read the material's name
				std::string materialName(materialNameLength, 0);
				stream.read(materialName.data(), materialNameLength);

				// Find the material in the given collection with said name
				for(const MaterialCollection::Item& item : materialCollection->GetItems()) {
					if(item.name == materialName) {
						items[i].material = item.material;
						break;
					}
				}
				if(!items[i].material)
					throw std::runtime_error("Invalid material name stored in render object file!");
			}
		}

		// Close the file stream
		stream.close();
	}
	void RenderObject::Save() const {
		// Open the file stream for writing
		std::ofstream stream(GetPath(), std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open render object file \"" + GetPath().string() + "\" for writing!");

		// Write the material collection's ID
		if(materialCollection) {
			BinaryWriteUint64(stream, materialCollection->GetID());
		} else {
			BinaryWriteUint64(stream, UINT64_T_MAX);
		}

		// Write the number of items
		BinaryWriteUint64(stream, items.size());

		for(size_t i = 0; i != items.size(); ++i) {
			// Write the item's name
			BinaryWriteUint64(stream, items[i].name.size());
			stream.write(items[i].name.data(), items[i].name.size());

			// Get the mesh's vertices and indices
			std::vector<RenderMesh::Vertex> vertices(items[i].mesh->GetVertexCount());
			std::vector<uint32_t> indices(items[i].mesh->GetIndexCount());

			items[i].mesh->GetMeshData(vertices.data(), indices.data());

			// Write all vertices
			BinaryWriteUint64(stream, vertices.size());

			for(size_t j = 0; j != vertices.size(); ++j) {
				BinaryWriteFloat(stream, vertices[j].position.x);
				BinaryWriteFloat(stream, vertices[j].position.y);
				BinaryWriteFloat(stream, vertices[j].position.z);

				BinaryWriteFloat(stream, vertices[j].uvCoord.x);
				BinaryWriteFloat(stream, vertices[j].uvCoord.y);

				BinaryWriteFloat(stream, vertices[j].normal.x);
				BinaryWriteFloat(stream, vertices[j].normal.y);
				BinaryWriteFloat(stream, vertices[j].normal.z);

				BinaryWriteFloat(stream, vertices[j].tangent.x);
				BinaryWriteFloat(stream, vertices[j].tangent.y);
				BinaryWriteFloat(stream, vertices[j].tangent.z);

				BinaryWriteFloat(stream, vertices[j].bitangent.x);
				BinaryWriteFloat(stream, vertices[j].bitangent.y);
				BinaryWriteFloat(stream, vertices[j].bitangent.z);
			}

			// Write all indices
			BinaryWriteUint64(stream, indices.size());

			for(size_t j = 0; j != indices.size(); ++j)
				BinaryWriteUint32(stream, indices[j]);
			
			if(items[i].material) {
				// Get the material's name from the material manager
				std::string materialName;
				for(const MaterialCollection::Item& item : materialCollection->GetItems()) {
					if(item.material == items[i].material) {
						materialName = item.name;
						break;
					}
				}

				// Write the material's name
				BinaryWriteUint64(stream, materialName.size());
				stream.write(materialName.data(), materialName.size());
			} else {
				// Write the material name's length as 0, to indicate that no material exists
				BinaryWriteUint64(stream, 0);
			}
		}

		// Close the file stream
		stream.close();
	}
	void RenderObject::Import() {
		// Destroy all previous items
		for(size_t i = 0; i != items.size(); ++i)
			delete items[i].mesh;
		items.clear();
		materialCollection = nullptr;

		// Open the file stream for reading
		std::ifstream stream(GetPath());
		if(!stream)
			throw std::runtime_error("Failed to open render object file \"" + GetPath().string() + "\" for reading!");
		
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

		// Get the directory the render object file is in
		std::filesystem::path fileDir = GetPath().lexically_normal().parent_path();

		// Save the current render mesh's info
		std::string currentName;
		std::string currentMaterialName;

		std::vector<Vec3f> positions;
		std::vector<Vec2f> uvCoords;
		std::vector<Vec3f> normals;
		std::unordered_map<Vec3f, uint32_t> tangentsMap;
		std::unordered_map<Vec3f, uint32_t> bitangentsMap;

		std::unordered_map<ArrVertex, uint32_t> arrVertices;
		std::vector<uint32_t> indices;

		// Parse every line in the file
		for(const std::string& line : lines) {
			// Create an input string stream
			std::istringstream strStream(line);

			// Read the first keyword
			std::string keyword;
			strStream >> keyword;

			if(keyword == "mtllib") {
				// Read the material collection file name
				std::string materialCollectionRelPath;
				strStream >> materialCollectionRelPath;

				// Get the material collection with the given path
				std::filesystem::path materialCollectionPath = (fileDir / materialCollectionRelPath).lexically_normal();
				materialCollection = dynamic_cast<MaterialCollection*>(GetProgram()->GetAssetManager()->GetAsset(materialCollectionPath));
				if(!materialCollection)
					throw std::runtime_error("Material collection file with path \"" + materialCollectionPath.string() + "\" not found!");
			} else if(keyword == "o") {
				// Build the current mesh, if it exists
				if(!currentName.empty()) {
					// Build the tangent and bitangent vectors
					std::vector<Vec3f> tangents(tangentsMap.size());
					for(const std::pair<const Vec3f, uint32_t>& tangentPair : tangentsMap)
						tangents[tangentPair.second] = tangentPair.first;

					std::vector<Vec3f> bitangents(bitangentsMap.size());
					for(const std::pair<const Vec3f, uint32_t>& bitangentPair : bitangentsMap)
						bitangents[bitangentPair.second] = bitangentPair.first;

					// Build the array of vertices
					std::vector<RenderMesh::Vertex> vertices(arrVertices.size());

					for(const std::pair<const ArrVertex, uint32_t>& arrVertexPair : arrVertices) {
						// Get the array vertex and target index
						ArrVertex arrVertex = arrVertexPair.first;
						uint32_t index = arrVertexPair.second;

						// Set the vertex's info
						vertices[index].position = positions[arrVertex.posIndex];
						vertices[index].uvCoord = uvCoords[arrVertex.uvIndex];
						vertices[index].normal = normals[arrVertex.normIndex];
						vertices[index].tangent = tangents[arrVertex.tanIndex];
						vertices[index].bitangent = bitangents[arrVertex.bitanIndex];
					}

					// Create the mesh
					RenderMesh* mesh = new RenderMesh(GetProgram()->GetRenderer(), vertices, indices);

					// Get the material with the current name
					Material* material = nullptr;
					if(!currentMaterialName.empty()) {
						for(const MaterialCollection::Item& item : materialCollection->GetItems()) {
							if(item.name == currentMaterialName) {
								material = item.material;
								break;
							}
						}
						if(!material)
							throw std::runtime_error("Invalid material name stored in render object file!");
					}
					
					// Add the new item to the vector
					items.push_back({ currentName, mesh, material });

					// Clear all of the current mesh's info
					currentName.clear();
					currentMaterialName.clear();

					positions.clear();
					uvCoords.clear();
					normals.clear();
					tangentsMap.clear();
					bitangentsMap.clear();

					arrVertices.clear();
					indices.clear();
				}

				// Read the new mesh's name
				strStream >> currentName;
			} else if(keyword == "v") {
				// Read the current position
				Vec3f position;
				strStream >> position.x >> position.y >> position.z;

				// Add the position to the array
				positions.push_back(position);
			} else if(keyword == "vt") {
				// Read the current UV coordinate
				Vec2f uvCoord;
				strStream >> uvCoord.x >> uvCoord.y;
				
				// Add the UV coordinate to the array
				uvCoords.push_back(uvCoord);
			} else if(keyword == "vn") {
				// Read the current normal and reduce it to a unit vector (not guaranteed by the Wavefront specification!)
				Vec3f normal;
				strStream >> normal.x >> normal.y >> normal.z;

				// Add the normal to the array
				normals.push_back(VecNormalized(normal));
			} else if(keyword == "f") {
				// Read the three index strings
				std::string face1, face2, face3;
				strStream >> face1 >> face2 >> face3;

				// Get the indices for each of the three vertices
				ArrVertex verts[3];

				char* str = face1.data();
				verts[0].posIndex = (uint32_t)strtoul(str, &str, 10) - 1;
				verts[0].uvIndex = (uint32_t)strtoul(str + 1, &str, 10) - 1;
				verts[0].normIndex = (uint32_t)strtoul(str + 1, &str, 10) - 1;
				
				str = face2.data();
				verts[1].posIndex = (uint32_t)strtoul(str, &str, 10) - 1;
				verts[1].uvIndex = (uint32_t)strtoul(str + 1, &str, 10) - 1;
				verts[1].normIndex = (uint32_t)strtoul(str + 1, &str, 10) - 1;

				str = face3.data();
				verts[2].posIndex = (uint32_t)strtoul(str, &str, 10) - 1;
				verts[2].uvIndex = (uint32_t)strtoul(str + 1, &str, 10) - 1;
				verts[2].normIndex = (uint32_t)strtoul(str + 1, &str, 10) - 1;

				// Check if the face was defined counter-clockwise, as required by the pipeline
				Vec3f totalNormal = normals[verts[0].normIndex] + normals[verts[1].normIndex] + normals[verts[2].normIndex];

				Vec3f edge1 = positions[verts[1].posIndex] - positions[verts[0].posIndex];
				Vec3f edge2 = positions[verts[2].posIndex] - positions[verts[0].posIndex];
				Vec3f crossNormal = VecCross(edge1, edge2);

				if(VecDot(totalNormal, crossNormal) < 0.0f) {
					// Swap two of the vertices
					ArrVertex aux = verts[1];
					verts[1] = verts[2];
					verts[2] = aux;

					// Swap the edges
					Vec3f edgeAux = edge1;
					edge1 = edge2;
					edge2 = edgeAux;
				}

				// Calculate the face's tangent and bitangent			
				Vec2f uvEdge1 = uvCoords[verts[1].uvIndex] - uvCoords[verts[0].uvIndex];
				Vec2f uvEdge2 = uvCoords[verts[2].uvIndex] - uvCoords[verts[0].uvIndex];
				
				float invDet = 1.0f / (uvEdge1.x * uvEdge2.y - uvEdge2.x * uvEdge1.y);

				Vec3f tangent = {
					(uvEdge2.y * edge1.x - uvEdge1.y * edge2.x) * invDet,
					(uvEdge2.y * edge1.y - uvEdge1.y * edge2.y) * invDet,
					(uvEdge2.y * edge1.z - uvEdge1.y * edge2.z) * invDet
				};
				Vec3f bitangent = {
					(-uvEdge2.x * edge1.x + uvEdge1.x * edge2.x) * invDet,
					(-uvEdge2.x * edge1.y + uvEdge1.x * edge2.y) * invDet,
					(-uvEdge2.x * edge1.z + uvEdge1.x * edge2.z) * invDet
				};

				// Get the indices for the tangent and bitangent
				uint32_t tanIndex;
				auto tanIter = tangentsMap.find(tangent);
				if(tanIter == tangentsMap.end()) {
					// Add the tangent to the map and set its index
					tanIndex = (uint32_t)tangentsMap.size();
					tangentsMap.insert({ tangent, tanIndex });
				} else {
					// Get the tangent's index
					tanIndex = tanIter->second;
				}

				uint32_t bitanIndex;
				auto bitanIter = bitangentsMap.find(bitangent);
				if(bitanIter == bitangentsMap.end()) {
					// Add the bitangent to the map and set its index
					bitanIndex = (uint32_t)bitangentsMap.size();
					bitangentsMap.insert({ bitangent, bitanIndex });
				} else {
					// Get the bitangent's index
					bitanIndex = bitanIter->second;
				}

				// Set the tangent and bitantent index in the array vertices
				for(uint32_t i = 0; i != 3; ++i) {
					verts[i].tanIndex = tanIndex;
					verts[i].bitanIndex = bitanIndex;
				}

				for(uint32_t i = 0; i != 3; ++i) {
					// Get the index for the current vertex
					uint32_t index;

					auto iter = arrVertices.find(verts[i]);
					if(iter == arrVertices.end()) {
						// Add the vertex to the map and set its index
						index = (uint32_t)arrVertices.size();
						arrVertices.insert({ verts[i], index });
					} else {
						// Get the vertex's index
						index = iter->second;
					}

					// Add the index to the array
					indices.push_back(index);
				}
			} else if(keyword == "usemtl") {
				// Get the material's name
				strStream >> currentMaterialName;
			}
		}

		// Build the last mesh, if it exists
		if(!currentName.empty()) {
			// Build the tangent and bitangent vectors
			std::vector<Vec3f> tangents(tangentsMap.size());
			for(const std::pair<const Vec3f, uint32_t>& tangentPair : tangentsMap)
				tangents[tangentPair.second] = tangentPair.first;

			std::vector<Vec3f> bitangents(bitangentsMap.size());
			for(const std::pair<const Vec3f, uint32_t>& bitangentPair : bitangentsMap)
				bitangents[bitangentPair.second] = bitangentPair.first;

			// Build the array of vertices
			std::vector<RenderMesh::Vertex> vertices(arrVertices.size());

			for(const std::pair<const ArrVertex, uint32_t>& arrVertexPair : arrVertices) {
				// Get the array vertex and target index
				ArrVertex arrVertex = arrVertexPair.first;
				uint32_t index = arrVertexPair.second;

				// Set the vertex's info
				vertices[index].position = positions[arrVertex.posIndex];
				vertices[index].uvCoord = uvCoords[arrVertex.uvIndex];
				vertices[index].normal = normals[arrVertex.normIndex];
				vertices[index].tangent = tangents[arrVertex.tanIndex];
				vertices[index].bitangent = bitangents[arrVertex.bitanIndex];
			}

			// Create the mesh
			RenderMesh* mesh = new RenderMesh(GetProgram()->GetRenderer(), vertices, indices);

			// Get the material with the current name
			Material* material = nullptr;
			if(!currentMaterialName.empty()) {
				for(const MaterialCollection::Item& item : materialCollection->GetItems()) {
					if(item.name == currentMaterialName) {
						material = item.material;
						break;
					}
				}
				if(!material)
					throw std::runtime_error("Invalid material name stored in render object file!");
			}
			
			// Add the new item to the vector
			items.push_back({ currentName, mesh, material });
		}
	}
	void RenderObject::Export() const {
		// Open the file stream for writing
		std::ofstream stream(GetPath());
		if(!stream)
			throw std::runtime_error("Failed to open render object file \"" + GetPath().string() + "\" for writing!");

		// Set the stream's floating point precision
		stream << std::fixed << std::setprecision(6);
		
		// Write an automatic comment to the file
		stream << "# " << WFE_ENGINE_NAME << " version " << WFE_ENGINE_VERSION_MAJOR << '.' << WFE_ENGINE_VERSION_MINOR << '.' << WFE_ENGINE_VERSION_PATCH << '\n';
		stream << "# Automatically generated OBJ file\n\n";

		// Get the directory the render object file is in
		std::filesystem::path fileDir = GetPath().lexically_normal().parent_path();

		// Write the material collection's path, if it exsists
		if(materialCollection) {
			std::filesystem::path materialCollectionPath = materialCollection->GetPath().lexically_relative(fileDir);
			stream << "mtllib " << materialCollectionPath.string() << "\n\n";
		}

		// Write every item's info
		for(size_t i = 0; i != items.size(); ++i) {
			// Write the object's name
			stream << "o " << items[i].name << '\n';

			// Write the material's name, if it exists
			if(items[i].material) {
				// Get the material's name from the material manager
				std::string materialName;
				for(const MaterialCollection::Item& item : materialCollection->GetItems()) {
					if(item.material == items[i].material) {
						materialName = item.name;
						break;
					}
				}

				stream << "\tusemtl " << materialName << "\n\n";
			}

			// Get the mesh's vertices and indices
			std::vector<RenderMesh::Vertex> vertices(items[i].mesh->GetVertexCount());
			std::vector<uint32_t> indices(items[i].mesh->GetIndexCount());

			items[i].mesh->GetMeshData(vertices.data(), indices.data());

			// Get all unique positions, UV coordinates and normals
			std::unordered_map<Vec3f, uint32_t> positions;
			std::unordered_map<Vec2f, uint32_t> uvCoords;
			std::unordered_map<Vec3f, uint32_t> normals;

			for(size_t j = 0; j != vertices.size(); ++j) {
				positions.insert({ vertices[j].position, 0 });
				uvCoords.insert({ vertices[j].uvCoord, 0 });
				normals.insert({ vertices[j].normal, 0 });
			}

			// Output all positions and set their indices
			size_t ind = 0;
			for(auto iter = positions.begin(); iter != positions.end(); ++iter) {
				stream << "\tv " << iter->first.x << ' ' << iter->first.y << ' ' << iter->first.z << '\n';
				iter->second = (uint32_t)++ind;
			}
			stream << '\n';

			// Output all UV coordinates and set their indices
			ind = 0;
			for(auto iter = uvCoords.begin(); iter != uvCoords.end(); ++iter) {
				stream << "\tvt " << iter->first.x << ' ' << iter->first.y << '\n';
				iter->second = (uint32_t)++ind;
			}
			stream << '\n';

			// Output all normals and set their indices
			ind = 0;
			for(auto iter = normals.begin(); iter != normals.end(); ++iter) {
				stream << "\tvn " << iter->first.x << ' ' << iter->first.y << ' ' << iter->first.z << '\n';
				iter->second = (uint32_t)++ind;
			}
			stream << '\n';

			// Generate all array vertices
			std::vector<ArrVertex> arrVertices(vertices.size());

			for(size_t j = 0; j != arrVertices.size(); ++j) {
				// Get the position, UV and normal index
				arrVertices[j].posIndex = positions[vertices[j].position];
				arrVertices[j].uvIndex = uvCoords[vertices[j].uvCoord];
				arrVertices[j].normIndex = normals[vertices[j].normal];
			}

			// Output all faces
			for(size_t j = 0; j != indices.size(); j += 3) {
				stream << "\tf ";
				for(size_t k = j; k != j + 3; ++k) {
					// Output the current array vertex
					ArrVertex arrVertex = arrVertices[indices[k]];
					stream << arrVertex.posIndex << '/' << arrVertex.uvIndex << '/' << arrVertex.normIndex << ' ';
				}
				stream << '\n';
			}
			stream << '\n';
		}

		// Close the file stream
		stream.close();
	}

	// Public functions
	RenderObject::~RenderObject() {
		// Destroy all items
		for(size_t i = 0; i != items.size(); ++i)
			delete items[i].mesh;
	}
}