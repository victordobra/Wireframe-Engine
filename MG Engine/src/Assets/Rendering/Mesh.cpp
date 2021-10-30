#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <vector>

#pragma region Mesh

Mesh::Mesh() : Positions(nullptr), PositionC(0), 
			   UVCoordinates(nullptr), UVCoordinateC(0), 
			   Normals(nullptr), NormalC(0),
			   Faces(nullptr), FaceC(0) { }
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

Mesh Mesh::FromObjFile(std::string FileLocation) {
	std::ifstream FileInput(FileLocation);

	if (!FileInput)
		throw std::runtime_error("No file found at the specified location!");

	std::vector<Vector3> PositionsV;
	std::vector<Vector2> UVCoordinatesV;
	std::vector<Vector3> NormalsV;
	std::vector<Face> FacesV;

	std::string CLine;
	while (std::getline(FileInput, CLine)) {
		if (CLine[0] == '#')
			continue;
		
		switch (CLine[0]) {
		case 'v':
			switch (CLine[1]) {
			case 't': 
				{
					std::string CNumS;
					Vector2 V;
					float* Adress = &V.X;

					for (size_t i = 3; i < CLine.length(); i++)
						if (CLine[i] == ' ') {
							*Adress = std::stof(CNumS);
							Adress++;

							CNumS = "";
						} else
							CNumS += CLine[i];

					UVCoordinatesV.push_back(V);
					break;
				}	
			case 'n': 
				{
					std::string CNumS;
					Vector3 V;
					float* Adress = &V.X;

					for (size_t i = 3; i < CLine.length(); i++)
						if (CLine[i] == ' ') {
							*Adress = std::stof(CNumS);
							Adress++;

							CNumS = "";
						} else
							CNumS += CLine[i];

					PositionsV.push_back(V);
					break;
				}
			default: 
				{
					std::string CNumS;
					Vector3 V;
					float* Adress = &V.X;

					for (size_t i = 2; i < CLine.length(); i++)
						if (CLine[i] == ' ') {
							*Adress = std::stof(CNumS);
							Adress++;

							CNumS = "";
						} else
							CNumS += CLine[i];

					NormalsV.push_back(V);
					break;
				}
			}

			break;
		case 'f':
			{
				std::vector<size_t> PositionIndices;
				std::vector<size_t> UVCoordinateIndices;
				size_t NormalIndex = 0;
				char ITypeIndex = 0;
				std::string CNumS;

				for (size_t i = 2; i < CLine.length(); i++)
					if (CLine[i] == ' ')
						ITypeIndex = 0;
					else if (CLine[i] == '/') {
						if (ITypeIndex == 0)
							PositionIndices.push_back((size_t)std::stoi(CNumS));
						else if (ITypeIndex == 1)
							UVCoordinateIndices.push_back((size_t)std::stoi(CNumS));
						else
							NormalIndex = (size_t)std::stoi(CNumS);

						CNumS = "";
						ITypeIndex = 0;
					}

				size_t VCount = PositionIndices.size();
				Face F(VCount);
				memcpy(F.PositionIndices, PositionIndices.data(), sizeof(size_t) * VCount);
				memcpy(F.UVCoordinateIndices, UVCoordinateIndices.data(), sizeof(size_t) * VCount);
				F.NormalIndex = NormalIndex;

				FacesV.push_back(F);
				break;
			}
		default:
			break;
		}
	}

	Mesh M;
	M.PositionC = PositionsV.size();
	M.UVCoordinateC = UVCoordinatesV.size();
	M.NormalC = NormalsV.size();
	M.FaceC = FacesV.size();

	M.Positions = new Vector3[M.PositionC];
	M.UVCoordinates = new Vector2[M.UVCoordinateC];
	M.Normals = new Vector3[M.NormalC];
	M.Faces = new Face[M.FaceC];

	memcpy(M.Positions, PositionsV.data(), M.PositionC * sizeof(Vector3));
	memcpy(M.UVCoordinates, PositionsV.data(), M.UVCoordinateC * sizeof(Vector2));
	memcpy(M.Normals, PositionsV.data(), M.NormalC * sizeof(Vector3));

	for (size_t i = 0; i < M.FaceC; i++)
		M.Faces[i] = FacesV[i];

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

bool Mesh::operator==(const Mesh& M) const {
	if (PositionC != M.PositionC || UVCoordinateC != M.UVCoordinateC || NormalC != M.NormalC || FaceC != M.FaceC)
		return false;

	if (!memcmp(Positions, M.Positions, sizeof(Vector3) * PositionC) ||
		!memcmp(UVCoordinates, M.UVCoordinates, sizeof(Vector2) * UVCoordinateC) ||
		!memcmp(Normals, M.Normals, sizeof(Vector3) * NormalC))
		return false;

	for (size_t i = 0; i < FaceC; i++)
		if (Faces[i] != M.Faces[i])
			return false;

	return true;
}
bool Mesh::operator==(Mesh&& M) const {
	if (PositionC != M.PositionC || UVCoordinateC != M.UVCoordinateC || NormalC != M.NormalC || FaceC != M.FaceC)
		return false;

	if (!memcmp(Positions, M.Positions, sizeof(Vector3) * PositionC) ||
		!memcmp(UVCoordinates, M.UVCoordinates, sizeof(Vector2) * UVCoordinateC) ||
		!memcmp(Normals, M.Normals, sizeof(Vector3) * NormalC))
		return false;

	for (size_t i = 0; i < FaceC; i++)
		if (Faces[i] != M.Faces[i])
			return false;

	return true;
}
bool Mesh::operator!=(const Mesh& M) const {
	return !operator==(M);
}
bool Mesh::operator!=(Mesh&& M) const {
	return !operator==(M);
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

#pragma endregion

#pragma region Face

Face::Face() : VertexCount(0),
			   PositionIndices(nullptr),
			   UVCoordinateIndices(nullptr),
			   NormalIndex(0) { }
Face::Face(const Face& F) : VertexCount(F.VertexCount), 
							PositionIndices(new size_t[F.VertexCount]),
							UVCoordinateIndices(new size_t[F.VertexCount]), 
							NormalIndex(F.NormalIndex) {

	memcpy(PositionIndices, F.PositionIndices, VertexCount * sizeof(size_t));
	memcpy(UVCoordinateIndices, F.UVCoordinateIndices, VertexCount * sizeof(size_t));
}
Face::Face(Face&& F) noexcept : VertexCount(F.VertexCount),
								PositionIndices(F.PositionIndices),
								UVCoordinateIndices(F.UVCoordinateIndices),
								NormalIndex(F.NormalIndex) { 
	
	F.PositionIndices = nullptr;
	F.UVCoordinateIndices = nullptr;
}
Face::Face(size_t VertexCount) : VertexCount(VertexCount),
									   PositionIndices(new size_t[VertexCount]),
									   UVCoordinateIndices(new size_t[VertexCount]),
									   NormalIndex(0) { }

Face& Face::operator=(const Face& F) {
	if (this == &F)
		return *this;

	delete[] PositionIndices;
	delete[] UVCoordinateIndices;

	VertexCount = F.VertexCount;
	PositionIndices = new size_t[VertexCount];
	UVCoordinateIndices = new size_t[VertexCount];
	NormalIndex = F.NormalIndex;

	memcpy(PositionIndices, F.PositionIndices, VertexCount * sizeof(size_t));
	memcpy(UVCoordinateIndices, F.UVCoordinateIndices, VertexCount * sizeof(size_t));

	return *this;
}
Face& Face::operator=(Face&& F) noexcept {
	delete[] PositionIndices;
	delete[] UVCoordinateIndices;

	VertexCount = F.VertexCount;
	PositionIndices = F.PositionIndices;
	UVCoordinateIndices = F.UVCoordinateIndices;
	NormalIndex = F.NormalIndex;

	F.PositionIndices = nullptr;
	F.UVCoordinateIndices = nullptr;

	return *this;
}

bool Face::operator==(const Face& F) const {
	if (VertexCount != F.VertexCount || NormalIndex != F.NormalIndex)
		return false;

	return memcmp(PositionIndices, F.PositionIndices, sizeof(size_t) * VertexCount) &&
		   memcmp(UVCoordinateIndices, F.UVCoordinateIndices, sizeof(size_t) * VertexCount);
}
bool Face::operator==(Face&& F) const {
	if (VertexCount != F.VertexCount || NormalIndex != F.NormalIndex)
		return false;

	return memcmp(PositionIndices, F.PositionIndices, sizeof(size_t) * VertexCount) &&
		memcmp(UVCoordinateIndices, F.UVCoordinateIndices, sizeof(size_t) * VertexCount);
}
bool Face::operator!=(const Face& F) const {
	return !operator==(F);
}
bool Face::operator!=(Face&& F) const {
	return !operator==(F);
}

std::string Face::ToString() const {
	return std::string("Face(") + std::to_string(VertexCount) + " vertices)";
}
size_t Face::GetHashCode() const {
	return typeid(Face).hash_code();
}

Face::~Face() {
	delete[] PositionIndices;
	delete[] UVCoordinateIndices;
}

#pragma endregion