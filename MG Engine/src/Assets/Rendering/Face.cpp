#include "Mesh.h"

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
