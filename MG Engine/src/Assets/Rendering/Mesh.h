#pragma once
#include "Asset.h"
#include "Vector3.h"
#include "Vector2.h"

class Face : public Object {
public:
	size_t* PositionIndices { nullptr };
	size_t* UVCoordinateIndices { nullptr };
	size_t NormalIndex;
	size_t VertexCount;

	Face() = default;
	Face(const Face& F);
	Face(Face&& F) noexcept;
	Face(size_t VertexCount);

	Face& operator=(const Face& F);
	Face& operator=(Face&& F) noexcept;

	bool operator==(const Face& M) const = delete;
	bool operator==(Face&& M) const = delete;
	bool operator!=(const Face& M) const = delete;
	bool operator!=(Face&& M) const = delete;

	std::string ToString() const override;
	size_t GetHashCode() const override;

	~Face();
};

class Mesh : public Asset {
public:
	Vector3* Positions { nullptr }; size_t PositionC;
	Vector2* UVCoordinates { nullptr }; size_t UVCoordinateC;
	Vector3* Normals { nullptr }; size_t NormalC;
	Face* Faces { nullptr }; size_t FaceC;

	Mesh() = default;
	Mesh(const Mesh& M);
	Mesh(Mesh&& M) noexcept;
	Mesh(std::string FileLocation);

	Mesh& operator=(const Mesh& F);
	Mesh& operator=(Mesh&& F) noexcept;

	bool operator==(const Mesh& M) const = delete;
	bool operator==(Mesh&& M) const = delete;
	bool operator!=(const Mesh& M) const = delete;
	bool operator!=(Mesh&& M) const = delete;

	void SaveToFile(std::string FileLocation) const override;
	static Mesh* FromObjFile(std::string FileLocation);

	std::string ToString() const override;
	size_t GetHashCode() const override;

	~Mesh();
};