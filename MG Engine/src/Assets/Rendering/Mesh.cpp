#include "Mesh.h"
#include "vector.h"
#include <fstream>
#include <sstream>
#include <vector>

namespace mge {
	Mesh::Mesh(const Mesh& other) : positions(new Vector3[other.positionC]), positionC(other.positionC), uvCoordinates(new Vector2[other.uvCoordinateC]), uvCoordinateC(other.uvCoordinateC), normals(new Vector3[other.normalC]), normalC(other.normalC), faces(new Face[other.faceC]), faceC(other.faceC) {
		memcpy(positions, other.positions, positionC * sizeof(Vector3));
		memcpy(uvCoordinates, other.uvCoordinates, uvCoordinateC * sizeof(Vector2));
		memcpy(normals, other.normals, normalC * sizeof(Vector3));
		for (size_t i = 0; i < faceC; i++)
			faces[i] = other.faces[i];
	}
	Mesh::Mesh(Mesh&& other) noexcept : positions(other.positions), positionC(other.positionC), uvCoordinates(other.uvCoordinates), uvCoordinateC(other.uvCoordinateC), normals(other.normals), normalC(other.positionC), faces(other.faces), faceC(other.faceC) {
		other.positions = nullptr;
		other.uvCoordinates = nullptr;
		other.normals = nullptr;
	}
	Mesh::Mesh(const char* fileLocation, AssetLoadResult& result) {
		std::ifstream fileInput(fileLocation);

		if (!fileInput) {
			result = AssetLoadResult::FILE_NOT_FOUND;
			return;
		}	

		vector<Vector3> positionsV;
		vector<Vector2> uvCoordinatesV;
		vector<Vector3> normalsV;
		vector<Face> facesV;

		std::string line;
		while (!fileInput.eof()) {
			std::getline(fileInput, line);
			if (line[0] == '#')
				continue;

			std::stringstream sStream(line);

			std::string init;
			sStream >> init;

			if (init == "v") {
				//The curent line is a vector position
				Vector3 vec;
				sStream >> vec.x >> vec.y >> vec.z;
				positionsV.push_back(vec);
			} else if (init == "vt") {
				//The curent line is a vector UV position
				Vector2 vec;
				sStream >> vec.x >> vec.y;
				uvCoordinatesV.push_back(vec);
			} else if (init == "vn") {
				//The curent line is a vector normal
				Vector3 vec;
				sStream >> vec.x >> vec.y >> vec.z;
				normalsV.push_back(vec);
			} else if (init == "f") {
				//The current line is a face
				vector<size_t> posIndV;
				vector<size_t> uvcIndV;
				size_t norInd = 0;

				while (!sStream.eof()) {
					//Read every vertex and set its indices
					std::string vertString;
					sStream >> vertString;
					std::stringstream vertStream(vertString);
					std::string pIndS, uvcIndS, nIndS;
					std::getline(vertStream, pIndS, '/');
					std::getline(vertStream, uvcIndS, '/');
					std::getline(vertStream, nIndS, ' ');

#ifdef _WIN64
					posIndV.push_back(std::stoull(pIndS) - 1);
					uvcIndV.push_back(std::stoull(uvcIndS) - 1);
					norInd = std::stoull(nIndS) - 1;
#else
					posIndV.push_back(std::stoul(pIndS) - 1);
					uvcIndV.push_back(std::stoul(uvcIndS) - 1);
					norInd = std::stoul(nIndS) - 1;
#endif
				}

				//Copy everything over
				Face face(posIndV.size());
				face.normalIndex = norInd;

				memcpy(face.positionIndices, posIndV.data(), sizeof(size_t) * face.vertexCount);
				memcpy(face.uvCoordinateIndices, uvcIndV.data(), sizeof(size_t) * face.vertexCount);

				facesV.push_back(face);
			}
		}

		if (!fileInput.good()) {
			result = AssetLoadResult::OTHER;
			return;
		}
		fileInput.close();

		//Copy everything from the vectors into the mesh
		positionC = positionsV.size();
		uvCoordinateC = uvCoordinatesV.size();
		normalC = normalsV.size();
		faceC = facesV.size();

		positions = new Vector3[positionC];
		uvCoordinates = new Vector2[uvCoordinateC];
		normals = new Vector3[normalC];
		faces = new Face[faceC];

		memcpy(positions, positionsV.data(), sizeof(Vector3) * positionC);
		memcpy(uvCoordinates, uvCoordinatesV.data(), sizeof(Vector2) * uvCoordinateC);
		memcpy(normals, normalsV.data(), sizeof(Vector3) * normalC);

		for (size_t i = 0; i < faceC; i++)
			faces[i] = facesV[i];

		result = AssetLoadResult::SUCCESS;
	}

	Mesh& Mesh::operator=(const Mesh& other) {
		if (this == &other)
			return *this;

		//Delete the old vectors
		delete[] positions;
		delete[] uvCoordinates;
		delete[] normals;
		delete[] faces;

		//Move over the new variables
		positionC = other.positionC;
		uvCoordinateC = other.uvCoordinateC;
		normalC = other.normalC;
		faceC = other.faceC;

		positions = new Vector3[positionC];
		uvCoordinates = new Vector2[uvCoordinateC];
		normals = new Vector3[normalC];
		faces = new Face[faceC];

		//Copy all of the heap allocated vectors
		memcpy(positions, other.positions, sizeof(Vector3) * positionC);
		memcpy(uvCoordinates, other.uvCoordinates, sizeof(Vector2) * uvCoordinateC);
		memcpy(normals, other.normals, sizeof(Vector3) * normalC);

		for (size_t i = 0; i < faceC; i++)
			faces[i] = other.faces[i];

		return *this;
	}
	Mesh& Mesh::operator=(Mesh&& other) noexcept {
		delete[] positions;
		delete[] uvCoordinates;
		delete[] normals;
		delete[] faces;

		positionC = other.positionC;
		uvCoordinateC = other.uvCoordinateC;
		normalC = other.normalC;
		faceC = other.faceC;

		positions = other.positions;
		uvCoordinates = other.uvCoordinates;
		normals = other.normals;
		faces = other.faces;

		other.positions = nullptr;
		other.uvCoordinates = nullptr;
		other.normals = nullptr;
		other.faces = nullptr;

		return *this;
	}

	AssetSaveResult Mesh::SaveToFile(const char* fileLocation) const {
		std::ofstream fileOutput(fileLocation, std::ios::binary);

		if (!fileOutput)
			return AssetSaveResult::FILE_NOT_FOUND;

		fileOutput << "#Saved with MG Engine's epic swag weed edition obj file saver which is very epic swag weed\n";

		//Output every position to the file
		for (size_t i = 0; i < positionC; i++)
			fileOutput << "v " << positions[i].x << " " << positions[i].y << positions[i].z << "\n";
		//Output every UV coordinate to the file
		for (size_t i = 0; i < uvCoordinateC; i++)
			fileOutput << "vt " << uvCoordinates[i].x << " " << uvCoordinates[i].y << "\n";
		//Output every normal to the file
		for (size_t i = 0; i < normalC; i++)
			fileOutput << "vn " << normals[i].x << " " << normals[i].y << normals[i].z << "\n";
		//Output every face to the file
		for (size_t i = 0; i < faceC; i++) {
			fileOutput << "f ";
			for (size_t j = 0; j < faces[i].vertexCount; j++)
				fileOutput << (faces[i].positionIndices[j] + 1) << "/" << (faces[i].uvCoordinateIndices + 1) << "/" << (faces[i].normalIndex + 1) << " ";
			fileOutput << "\n";
		}

		if (!fileOutput.good())
			return AssetSaveResult::OTHER;

		fileOutput.close();

		return AssetSaveResult::SUCCESS;
	}
}