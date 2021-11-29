#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <vector>

Mesh::Mesh(const Mesh& M) : Positions(new Vector3[M.PositionC]), PositionC(M.PositionC), 
							UVCoordinates(new Vector2[M.UVCoordinateC]), UVCoordinateC(M.UVCoordinateC), 
							Normals(new Vector3[M.NormalC]), NormalC(M.NormalC),
							Faces(new Face[M.FaceC]), FaceC(M.FaceC) {

	memcpy(Positions, M.Positions, PositionC * sizeof(Vector3));
	memcpy(UVCoordinates, M.UVCoordinates, UVCoordinateC * sizeof(Vector2));
	memcpy(Normals, M.Normals, NormalC * sizeof(Vector3));
	for (size_t i = 0; i < FaceC; i++)
		Faces[i] = M.Faces[i];
}
Mesh::Mesh(Mesh&& M) noexcept : Positions(M.Positions), PositionC(M.PositionC), 
								UVCoordinates(M.UVCoordinates), UVCoordinateC(M.UVCoordinateC), 
								Normals(M.Normals), NormalC(M.PositionC),
								Faces(M.Faces), FaceC(M.FaceC) { 

	M.Positions = nullptr; 
	M.UVCoordinates = nullptr;
	M.Normals = nullptr;
}
Mesh::Mesh(std::string FileLocation) {
	std::ifstream FileInput(FileLocation, std::ios::binary);

	if (!FileInput)
		throw std::runtime_error("No file found at the specified location!");

	FileInput.read((char*)(&this->PositionC), sizeof(size_t));
	FileInput.read((char*)(&this->UVCoordinateC), sizeof(size_t));
	FileInput.read((char*)(&this->NormalC), sizeof(size_t));

	this->Positions = new Vector3[this->PositionC];
	this->UVCoordinates = new Vector2[this->UVCoordinateC];
	this->Normals = new Vector3[this->NormalC];

	FileInput.read((char*)this->Positions, (std::streamsize)this->PositionC * sizeof(Vector3));
	FileInput.read((char*)this->UVCoordinates, (std::streamsize)this->UVCoordinateC * sizeof(Vector2));
	FileInput.read((char*)this->Normals, (std::streamsize)this->NormalC * sizeof(Vector3));

	FileInput.read((char*)(&this->FaceC), sizeof(size_t));
	this->Faces = new Face[this->FaceC];

	for (size_t i = 0; i < this->FaceC; i++) {
		FileInput.read((char*)(&this->Faces[i].VertexCount), sizeof(size_t));

		this->Faces[i].PositionIndices = (size_t*)malloc(this->Faces[i].VertexCount * sizeof(size_t));
		this->Faces[i].UVCoordinateIndices = (size_t*)malloc(this->Faces[i].VertexCount * sizeof(size_t));

		FileInput.read((char*)this->Faces[i].PositionIndices, (std::streamsize)this->Faces[i].VertexCount * sizeof(size_t));
		FileInput.read((char*)this->Faces[i].UVCoordinateIndices, (std::streamsize)this->Faces[i].VertexCount * sizeof(size_t));
		FileInput.read((char*)(&this->Faces[i].NormalIndex), (std::streamsize)sizeof(size_t));
	}

	if (!FileInput.good())
		throw std::runtime_error("Error occured at reading time!");

	FileInput.close();
}

void Mesh::SaveToFile(std::string FileLocation) const {
	std::ofstream FileOutput(FileLocation, std::ios::binary);

	if (!FileOutput)
		throw std::runtime_error("No file found at the specified location!");

	FileOutput.write((char*)(&this->PositionC), sizeof(size_t));
	FileOutput.write((char*)(&this->UVCoordinateC), sizeof(size_t));
	FileOutput.write((char*)(&this->NormalC), sizeof(size_t));

	FileOutput.write((char*)this->Positions, (std::streamsize)this->PositionC * sizeof(Vector3));
	FileOutput.write((char*)this->UVCoordinates, (std::streamsize)this->UVCoordinateC * sizeof(Vector2));
	FileOutput.write((char*)this->Normals, (std::streamsize)this->NormalC * sizeof(Vector3));

	FileOutput.write((char*)(&this->FaceC), sizeof(size_t));

	for (size_t i = 0; i < this->FaceC; i++) {
		FileOutput.write((char*)(&this->Faces[i].VertexCount), sizeof(size_t));

		FileOutput.write((char*)this->Faces[i].PositionIndices, (std::streamsize)this->Faces[i].VertexCount * sizeof(size_t));
		FileOutput.write((char*)this->Faces[i].UVCoordinateIndices, (std::streamsize)this->Faces[i].VertexCount * sizeof(size_t));
		FileOutput.write((char*)(&this->Faces[i].NormalIndex), (std::streamsize)sizeof(size_t));
	}

	if (!FileOutput.good())
		throw std::runtime_error("Error occured at writing time!");

	FileOutput.close();
}

Mesh* Mesh::FromObjFile(std::string FileLocation) {
	std::ifstream FileInput(FileLocation);

	if (!FileInput)
		throw std::runtime_error("No file found at the specified location!");

	std::vector<Vector3> PositionsV;
	std::vector<Vector2> UVCoordinatesV;
	std::vector<Vector3> NormalsV;
	std::vector<Face> FacesV;

	std::string CLine;
	while (!FileInput.eof()) {
		std::getline(FileInput, CLine);
		if (CLine[0] == '#')
			continue;

		std::stringstream SStream(CLine);
		
		std::string Init;
		SStream >> Init;

		if (Init == "v") {
			Vector3 V;
			SStream >> V.X >> V.Y >> V.Z;
			PositionsV.push_back(V);
		} else if (Init == "vt") {
			Vector2 V;
			SStream >> V.X >> V.Y;
			UVCoordinatesV.push_back(V);
		} else if (Init == "vn") {
			Vector3 V;
			SStream >> V.X >> V.Y >> V.Z;
			NormalsV.push_back(V);
		} else if (Init == "f") {
			std::vector<size_t> PosIndV;
			std::vector<size_t> UVCIndV;
			int NorInd = 0;

			while (!SStream.eof()) {
				std::string TempString;
				SStream >> TempString;
				std::stringstream TempStream(TempString);
				std::string PIndS, UVCIndS, NIndS;
				std::getline(TempStream, PIndS, '/');
				std::getline(TempStream, UVCIndS, '/');
				std::getline(TempStream, NIndS, ' ');

#ifdef _WIN64
				PosIndV.push_back(std::stoull(PIndS) - 1);
				UVCIndV.push_back(std::stoull(UVCIndS) - 1);
#else
				PosIndV.push_back(std::stoul(PIndS) - 1);
				UVCIndV.push_back(std::stoul(UVCIndS) - 1);
#endif
				NorInd = std::stoi(NIndS) - 1;
			}

			Face F(PosIndV.size());
			F.NormalIndex = NorInd;

			memcpy(F.PositionIndices, PosIndV.data(), sizeof(size_t) * F.VertexCount);
			memcpy(F.UVCoordinateIndices, UVCIndV.data(), sizeof(size_t) * F.VertexCount);

			FacesV.push_back(F);
		}
	}

	Mesh* M = new Mesh();
	M->PositionC = PositionsV.size();
	M->UVCoordinateC = UVCoordinatesV.size();
	M->NormalC = NormalsV.size();
	M->FaceC = FacesV.size();

	M->Positions = new Vector3[M->PositionC];
	M->UVCoordinates = new Vector2[M->UVCoordinateC];
	M->Normals = new Vector3[M->NormalC];
	M->Faces = new Face[M->FaceC];

	memcpy(M->Positions, PositionsV.data(), sizeof(Vector3) * M->PositionC);
	memcpy(M->UVCoordinates, UVCoordinatesV.data(), sizeof(Vector2) * M->UVCoordinateC);
	memcpy(M->Normals, NormalsV.data(), sizeof(Vector3) * M->NormalC);

	for (size_t i = 0; i < M->FaceC; i++)
		M->Faces[i] = FacesV[i];

	return M;
}

Mesh& Mesh::operator=(const Mesh& M) {
	if (this == &M)
		return *this;

	delete[] Positions;
	delete[] UVCoordinates;
	delete[] Normals;
	delete[] Faces;

	PositionC = M.PositionC;
	UVCoordinateC = M.UVCoordinateC;
	NormalC = M.NormalC;
	FaceC = M.FaceC;

	Positions = new Vector3[PositionC];
	UVCoordinates = new Vector2[UVCoordinateC];
	Normals = new Vector3[NormalC];
	Faces = new Face[FaceC];

	memcpy(Positions, M.Positions, sizeof(Vector3) * PositionC);
	memcpy(UVCoordinates, M.UVCoordinates, sizeof(Vector2) * UVCoordinateC);
	memcpy(Normals, M.Normals, sizeof(Vector3) * NormalC);

	for (size_t i = 0; i < FaceC; i++)
		Faces[i] = M.Faces[i];

	return *this;
}
Mesh& Mesh::operator=(Mesh&& M) noexcept {
	delete[] Positions;
	delete[] UVCoordinates;
	delete[] Normals;
	delete[] Faces;

	PositionC = M.PositionC;
	UVCoordinateC = M.UVCoordinateC;
	NormalC = M.NormalC;
	FaceC = M.FaceC;

	Positions = M.Positions;
	UVCoordinates = M.UVCoordinates;
	Normals = M.Normals;
	Faces = M.Faces;

	M.Positions = nullptr;
	M.UVCoordinates = nullptr;
	M.Normals = nullptr;
	M.Faces = nullptr;

	return *this;
}

std::string Mesh::ToString() const {
	return std::string("Mesh(") + std::to_string(FaceC) + " faces)";
}
size_t Mesh::GetHashCode() const {
	return typeid(Mesh).hash_code();
}

Mesh::~Mesh() {
	delete[] Positions;
	delete[] UVCoordinates;
	delete[] Normals;
	delete[] Faces;
}
