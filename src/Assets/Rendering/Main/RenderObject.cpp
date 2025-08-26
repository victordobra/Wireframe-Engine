#include "RenderObject.hpp"
#include "Core/Math/General/VecUtils.hpp"
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
	// Structs
	struct MeshData {
		std::vector<RenderMesh::Vertex> vertices;
		std::vector<uint32_t> indices;
	};

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
		uint64_t materialCollectionID = BinaryReadUint64BE(stream);

		if(materialCollectionID != UINT64_T_MAX) {
			// Get the material collection
			materialCollection = dynamic_cast<MaterialCollection*>(GetProgram()->GetAssetManager()->GetAsset(materialCollectionID));
			if(!materialCollection)
				throw std::runtime_error("Invalid material collection ID stored in render object file!");
		} else {
			materialCollection = nullptr;
		}

		// Read the number of items
		uint64_t itemCount = BinaryReadUint64BE(stream);
		items.resize(itemCount);

		for(size_t i = 0; i != itemCount; ++i) {
			// Read the item's name
			uint64_t nameLength = BinaryReadUint64BE(stream);
			items[i].name.resize(nameLength);
			stream.read(items[i].name.data(), nameLength);

			// Read the number of vertices
			uint64_t vertexCount = BinaryReadUint64BE(stream);
			std::vector<RenderMesh::Vertex> vertices(vertexCount);

			// Read all vertices
			for(size_t j = 0; j != vertexCount; ++j) {
				vertices[j].position.x = BinaryReadFloatBE(stream);
				vertices[j].position.y = BinaryReadFloatBE(stream);
				vertices[j].position.z = BinaryReadFloatBE(stream);

				vertices[j].uvCoord.x = BinaryReadFloatBE(stream);
				vertices[j].uvCoord.y = BinaryReadFloatBE(stream);

				vertices[j].normal.x = BinaryReadFloatBE(stream);
				vertices[j].normal.y = BinaryReadFloatBE(stream);
				vertices[j].normal.z = BinaryReadFloatBE(stream);

				vertices[j].tangent.x = BinaryReadFloatBE(stream);
				vertices[j].tangent.y = BinaryReadFloatBE(stream);
				vertices[j].tangent.z = BinaryReadFloatBE(stream);

				vertices[j].bitangent.x = BinaryReadFloatBE(stream);
				vertices[j].bitangent.y = BinaryReadFloatBE(stream);
				vertices[j].bitangent.z = BinaryReadFloatBE(stream);
			}

			// Read the number of indices
			uint64_t indexCount = BinaryReadUint64BE(stream);
			std::vector<uint32_t> indices(indexCount);

			// Read all indices
			for(size_t j = 0; j != indexCount; ++j)
				indices[j] = BinaryReadUint32BE(stream);

			// Create the mesh
			items[i].mesh = new RenderMesh(GetProgram()->GetRenderer(), vertices, indices);
			
			// Read the material name's length
			uint64_t materialNameLength = BinaryReadUint64BE(stream);

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

			// Read the flat index count
			items[i].flatIndexCount = BinaryReadUint64BE(stream);
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
			BinaryWriteUint64BE(stream, materialCollection->GetID());
		} else {
			BinaryWriteUint64BE(stream, UINT64_T_MAX);
		}

		// Write the number of items
		BinaryWriteUint64BE(stream, items.size());

		for(size_t i = 0; i != items.size(); ++i) {
			// Write the item's name
			BinaryWriteUint64BE(stream, items[i].name.size());
			stream.write(items[i].name.data(), items[i].name.size());

			// Get the mesh's vertices and indices
			std::vector<RenderMesh::Vertex> vertices(items[i].mesh->GetVertexCount());
			std::vector<uint32_t> indices(items[i].mesh->GetIndexCount());

			items[i].mesh->GetMeshData(vertices.data(), indices.data());

			// Write all vertices
			BinaryWriteUint64BE(stream, vertices.size());

			for(size_t j = 0; j != vertices.size(); ++j) {
				BinaryWriteFloatBE(stream, vertices[j].position.x);
				BinaryWriteFloatBE(stream, vertices[j].position.y);
				BinaryWriteFloatBE(stream, vertices[j].position.z);

				BinaryWriteFloatBE(stream, vertices[j].uvCoord.x);
				BinaryWriteFloatBE(stream, vertices[j].uvCoord.y);

				BinaryWriteFloatBE(stream, vertices[j].normal.x);
				BinaryWriteFloatBE(stream, vertices[j].normal.y);
				BinaryWriteFloatBE(stream, vertices[j].normal.z);

				BinaryWriteFloatBE(stream, vertices[j].tangent.x);
				BinaryWriteFloatBE(stream, vertices[j].tangent.y);
				BinaryWriteFloatBE(stream, vertices[j].tangent.z);

				BinaryWriteFloatBE(stream, vertices[j].bitangent.x);
				BinaryWriteFloatBE(stream, vertices[j].bitangent.y);
				BinaryWriteFloatBE(stream, vertices[j].bitangent.z);
			}

			// Write all indices
			BinaryWriteUint64BE(stream, indices.size());

			for(size_t j = 0; j != indices.size(); ++j)
				BinaryWriteUint32BE(stream, indices[j]);
			
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
				BinaryWriteUint64BE(stream, materialName.size());
				stream.write(materialName.data(), materialName.size());
			} else {
				// Write the material name's length as 0, to indicate that no material exists
				BinaryWriteUint64BE(stream, 0);
			}

			// Write the flat index count
			BinaryWriteUint64BE(stream, items[i].flatIndexCount);
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
		std::vector<uint32_t> smoothIndices;

		bool smoothShading = false;

		// Parse every line in the file
		for(size_t i = 0; i != lines.size(); ++i) {
			// Create an input string stream
			std::istringstream strStream(lines[i]);

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

				if(smoothShading) {
					// Set the tangent and bitangent indices to an undefined value
					for(uint32_t j = 0; j != 3; ++j) {
						verts[j].tanIndex = UINT32_T_MAX;
						verts[j].bitanIndex = UINT32_T_MAX;
					}

					for(uint32_t j = 0; j != 3; ++j) {
						// Get the index for the current vertex
						uint32_t index;

						auto iter = arrVertices.find(verts[j]);
						if(iter == arrVertices.end()) {
							// Add the vertex to the map and set its index
							index = (uint32_t)arrVertices.size();
							arrVertices.insert({ verts[j], index });
						} else {
							// Get the vertex's index
							index = iter->second;
						}

						// Add the index to the array
						smoothIndices.push_back(index);
					}
				} else {
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

					// Normalize the tangent and bitangent vectors
					tangent = VecNormalized(tangent);
					bitangent = VecNormalized(bitangent);

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
					for(uint32_t j = 0; j != 3; ++j) {
						verts[j].tanIndex = tanIndex;
						verts[j].bitanIndex = bitanIndex;
					}

					for(uint32_t j = 0; j != 3; ++j) {
						// Get the index for the current vertex
						uint32_t index;

						auto iter = arrVertices.find(verts[j]);
						if(iter == arrVertices.end()) {
							// Add the vertex to the map and set its index
							index = (uint32_t)arrVertices.size();
							arrVertices.insert({ verts[j], index });
						} else {
							// Get the vertex's index
							index = iter->second;
						}

						// Add the index to the array
						indices.push_back(index);
					}
				}
			} else if(keyword == "usemtl") {
				// Get the material's name
				strStream >> currentMaterialName;
			} else if(keyword == "s") {
				// Get the smooth shading property
				std::string enabled;
				strStream >> enabled;

				smoothShading = enabled == "1";
			}

			// Check if the current mesh needs to be build			
			if(keyword == "o" || i == lines.size() - 1) {
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

						if(arrVertex.tanIndex != UINT32_T_MAX && arrVertex.tanIndex != UINT32_T_MAX) {
							vertices[index].tangent = tangents[arrVertex.tanIndex];
							vertices[index].bitangent = bitangents[arrVertex.bitanIndex];
						} else {
							vertices[index].tangent = VEC3F_ZERO;
							vertices[index].bitangent = VEC3F_ZERO;
						}
					}

					// Calculate the tangents and bitangents of the smooth shaded vertices
					std::vector<float> totalAngle(vertices.size(), 0.0f);

					for(size_t j = 0; j != smoothIndices.size(); j += 3) {
						// Calculate the tangent and bitangent for the current face
						size_t ind1 = smoothIndices[j], ind2 = smoothIndices[j + 1], ind3 = smoothIndices[j + 2];

						Vec3f edge1 = vertices[ind2].position - vertices[ind1].position;
						Vec3f edge2 = vertices[ind3].position - vertices[ind1].position;

						Vec2f uvEdge1 = vertices[ind2].uvCoord - vertices[ind1].uvCoord;
						Vec2f uvEdge2 = vertices[ind3].uvCoord - vertices[ind1].uvCoord;
						
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

						// Add the tangent and bitangent to the weighted average in every vertex
						float angle1 = VecAngle(edge1, edge2);
						float angle2 = VecAngle(-edge1, vertices[ind3].position - vertices[ind2].position);
						float angle3 = PI_F - angle1 - angle2;

						vertices[ind1].tangent += tangent * angle1;
						vertices[ind1].bitangent += bitangent * angle1;
						totalAngle[ind1] += angle1;

						vertices[ind2].tangent += tangent * angle2;
						vertices[ind2].bitangent += bitangent * angle2;
						totalAngle[ind2] += angle2;

						vertices[ind3].tangent += tangent * angle3;
						vertices[ind3].bitangent += bitangent * angle3;
						totalAngle[ind3] += angle3;
					}
					for(size_t j = 0; j != vertices.size(); ++j) {
						// Skip the current vertex if its tangent space is flat shaded
						if(!totalAngle[j])
							continue;
						
						// Average out the tangent and bitangent
						vertices[j].tangent /= totalAngle[j];
						vertices[j].bitangent /= totalAngle[j];

						// Modify the tangent and bitangent to be perpendicular to the normal
						vertices[j].bitangent = VecNormalized(VecCross(vertices[j].normal, vertices[j].tangent));
						vertices[j].tangent = VecNormalized(VecCross(vertices[j].bitangent, vertices[j].normal));
					}

					// Add the smooth indices to the index vector
					indices.insert(indices.end(), smoothIndices.begin(), smoothIndices.end());

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
					items.push_back({ currentName, mesh, material, indices.size() - smoothIndices.size() });

					// Clear all of the current mesh's info
					currentName.clear();
					currentMaterialName.clear();

					arrVertices.clear();
					indices.clear();
					smoothIndices.clear();
				}

				if(keyword == "o") {
					// Read the new mesh's name
					strStream >> currentName;
				}
			}
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

		// Get the vertices and indices for all meshes
		std::vector<MeshData> meshes(items.size());
		for(size_t i = 0; i != items.size(); ++i) {
			// Get the mesh's vertices and indices
			meshes[i].vertices.resize(items[i].mesh->GetVertexCount());
			meshes[i].indices.resize(items[i].mesh->GetIndexCount());

			items[i].mesh->GetMeshData(meshes[i].vertices.data(), meshes[i].indices.data());
		}

		// Get all unique positions, UV coordinates and normals
		std::unordered_map<Vec3f, uint32_t> positions;
		std::unordered_map<Vec2f, uint32_t> uvCoords;
		std::unordered_map<Vec3f, uint32_t> normals;

		for(size_t i = 0; i != meshes.size(); ++i) {
			for(size_t j = 0; j != meshes[i].vertices.size(); ++j) {
				positions.insert({ meshes[i].vertices[j].position, 0 });
				uvCoords.insert({ meshes[i].vertices[j].uvCoord, 0 });
				normals.insert({ meshes[i].vertices[j].normal, 0 });
			}
		}

		// Output all positions and set their indices
		size_t ind = 0;
		for(auto iter = positions.begin(); iter != positions.end(); ++iter) {
			stream << "v " << iter->first.x << ' ' << iter->first.y << ' ' << iter->first.z << '\n';
			iter->second = (uint32_t)++ind;
		}
		stream << '\n';

		// Output all UV coordinates and set their indices
		ind = 0;
		for(auto iter = uvCoords.begin(); iter != uvCoords.end(); ++iter) {
			stream << "vt " << iter->first.x << ' ' << iter->first.y << '\n';
			iter->second = (uint32_t)++ind;
		}
		stream << '\n';

		// Output all normals and set their indices
		ind = 0;
		for(auto iter = normals.begin(); iter != normals.end(); ++iter) {
			stream << "vn " << iter->first.x << ' ' << iter->first.y << ' ' << iter->first.z << '\n';
			iter->second = (uint32_t)++ind;
		}
		stream << '\n';

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

			// Generate all array vertices
			std::vector<ArrVertex> arrVertices(meshes[i].vertices.size());

			for(size_t j = 0; j != arrVertices.size(); ++j) {
				// Get the position, UV and normal index
				arrVertices[j].posIndex = positions[meshes[i].vertices[j].position];
				arrVertices[j].uvIndex = uvCoords[meshes[i].vertices[j].uvCoord];
				arrVertices[j].normIndex = normals[meshes[i].vertices[j].normal];
			}

			if(items[i].flatIndexCount) {
				// Output all flat faces
				stream << "\ts 0\n";
				for(size_t j = 0; j != items[i].flatIndexCount; j += 3) {
					stream << "\tf ";
					for(size_t k = j; k != j + 3; ++k) {
						// Output the current array vertex
						ArrVertex arrVertex = arrVertices[meshes[i].indices[k]];
						stream << arrVertex.posIndex << '/' << arrVertex.uvIndex << '/' << arrVertex.normIndex << ' ';
					}
					stream << '\n';
				}
				stream << '\n';
			}

			if(items[i].flatIndexCount != meshes[i].indices.size()) {
				// Output all smooth faces
				stream << "\ts 1\n";
				for(size_t j = items[i].flatIndexCount; j != meshes[i].indices.size(); j += 3) {
					stream << "\tf ";
					for(size_t k = j; k != j + 3; ++k) {
						// Output the current array vertex
						ArrVertex arrVertex = arrVertices[meshes[i].indices[k]];
						stream << arrVertex.posIndex << '/' << arrVertex.uvIndex << '/' << arrVertex.normIndex << ' ';
					}
					stream << '\n';
				}
				stream << '\n';
			}
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