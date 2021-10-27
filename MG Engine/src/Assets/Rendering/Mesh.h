#pragma once
#include "Asset.h"
#include "Vector3.h"
#include "Vector2.h"

class Face;

class Mesh : public Asset {
public:
	Vector3* Positions; unsigned int PositionC;
	Vector2* UVCoordinates; unsigned int UVCoordinateC;
	Vector3* Normals; unsigned int NormalC;
	Face* Faces; unsigned int FaceC;

	Mesh();
	Mesh(const Mesh& M);
	Mesh(Mesh&& M) noexcept;
	Mesh(std::string FileLocation);

	Mesh& operator=(const Mesh& F);
	Mesh& operator=(Mesh&& F) noexcept;

	bool operator==(const Mesh& M) const;
	bool operator==(Mesh&& M) const;
	bool operator!=(const Mesh& M) const;
	bool operator!=(Mesh&& M) const;

	void SaveToFile(std::string FileLocation) const override;
	static Mesh FromObjFile(std::string FileLocation);

	std::string ToString() const override;
	int GetHashCode() const override;

	~Mesh();
};

class Face : public Object {
public:
	unsigned int* PositionIndices;
	unsigned int* UVCoordinateIndices;
	unsigned int NormalIndex;
	unsigned int VertexCount;

	Face();
	Face(const Face& F);
	Face(Face&& F) noexcept;
	Face(unsigned int VertexCount);

	Face& operator=(const Face& F);
	Face& operator=(Face&& F) noexcept;

	bool operator==(const Face& M) const;
	bool operator==(Face&& M) const;
	bool operator!=(const Face& M) const;
	bool operator!=(Face&& M) const;
	
	std::string ToString() const override;
	int GetHashCode() const override;

	~Face();
};