#pragma once
#include "Asset.h"
#include "Vector3.h"
#include "Vector2.h"

namespace mge {
	class Face {
	public:
		size_t* positionIndices{ nullptr };
		size_t* uvCoordinateIndices{ nullptr };
		size_t normalIndex{};
		size_t vertexCount{};

		Face() = default;
		Face(const Face& other) : vertexCount(other.vertexCount), positionIndices(new size_t[other.vertexCount]), uvCoordinateIndices(new size_t[other.vertexCount]), normalIndex(other.normalIndex) {
			memcpy(positionIndices, other.positionIndices, vertexCount * sizeof(size_t));
			memcpy(uvCoordinateIndices, other.uvCoordinateIndices, vertexCount * sizeof(size_t));
		}
		Face(Face&& other) noexcept : vertexCount(other.vertexCount), positionIndices(other.positionIndices), uvCoordinateIndices(other.uvCoordinateIndices), normalIndex(other.normalIndex) {
			other.positionIndices = nullptr;
			other.uvCoordinateIndices = nullptr;
		}
		explicit Face(size_t vertexCount) : vertexCount(vertexCount), positionIndices(new size_t[vertexCount]), uvCoordinateIndices(new size_t[vertexCount]), normalIndex(0) { }

		Face& operator=(const Face& other);
		Face& operator=(Face&& other) noexcept;

		bool operator==(const Face&) const = delete;
		bool operator==(Face&&) const = delete;
		bool operator!=(const Face&) const = delete;
		bool operator!=(Face&&) const = delete;

		~Face() {
			delete[] positionIndices;
			delete[] uvCoordinateIndices;
		}
	};

	class Mesh : public Asset {
	public:
		Vector3* positions{ nullptr }; size_t positionC{};
		Vector2* uvCoordinates{ nullptr }; size_t uvCoordinateC{};
		Vector3* normals{ nullptr }; size_t normalC{};
		Face* faces{ nullptr }; size_t faceC{};

		Mesh() = default;
		Mesh(const Mesh& other);
		Mesh(Mesh&& other) noexcept;
		Mesh(const char* fileLocation, AssetLoadResult& result);

		Mesh& operator=(const Mesh& other);
		Mesh& operator=(Mesh&& other) noexcept;

		bool operator==(const Mesh&) const = delete;
		bool operator==(Mesh&&) const = delete;
		bool operator!=(const Mesh&) const = delete;
		bool operator!=(Mesh&&) const = delete;

		AssetSaveResult SaveToFile(const char* fileLocation) const override;

		~Mesh() {
			delete[] positions;
			delete[] uvCoordinates;
			delete[] normals;
			delete[] faces;
		}
	};
}