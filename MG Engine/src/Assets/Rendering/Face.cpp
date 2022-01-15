#include "Mesh.h"
#include <vector>

namespace mge {
	Face& Face::operator=(const Face& other) {
		if (this == &other)
			return *this;

		delete[] positionIndices;
		delete[] uvCoordinateIndices;

		vertexCount = other.vertexCount;
		positionIndices = new size_t[vertexCount];
		uvCoordinateIndices = new size_t[vertexCount];
		normalIndex = other.normalIndex;

		memcpy(positionIndices, other.positionIndices, vertexCount * sizeof(size_t));
		memcpy(uvCoordinateIndices, other.uvCoordinateIndices, vertexCount * sizeof(size_t));

		return *this;
	}
	Face& Face::operator=(Face&& other) noexcept {
		delete[] positionIndices;
		delete[] uvCoordinateIndices;

		vertexCount = other.vertexCount;
		positionIndices = other.positionIndices;
		uvCoordinateIndices = other.uvCoordinateIndices;
		normalIndex = other.normalIndex;

		other.positionIndices = nullptr;
		other.uvCoordinateIndices = nullptr;

		return *this;
	}
}
