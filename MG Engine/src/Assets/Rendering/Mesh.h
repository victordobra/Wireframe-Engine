#pragma once
#include "Asset.h"
#include "Vector3.h"
#include "Vector2.h"

namespace mge {
	class Vertex {
	public:
		Vector3 position{};
		Vector2 uvCoordinate{};
		Vector3 normal{};

		Vertex() = default;
		Vertex(const Vertex&) = default;
		Vertex(Vertex&&) noexcept = default;
		Vertex(Vector3 position, Vector2 uvCoordinate, Vector3 normal) : position(position), uvCoordinate(uvCoordinate), normal(normal) { }

		inline Vertex& operator=(const Vertex&) = default;
		inline Vertex& operator=(Vertex&&) noexcept = default;

		inline bool operator==(const Vertex& other) const { return position == other.position && uvCoordinate == other.uvCoordinate && normal == other.normal; }
		inline bool operator==(Vertex&& other) const { return position == other.position && uvCoordinate == other.uvCoordinate && normal == other.normal; }
		inline bool operator!=(const Vertex& other) const { return position != other.position || uvCoordinate != other.uvCoordinate || normal != other.normal; }
		inline bool operator!=(Vertex&& other) const { return position != other.position || uvCoordinate != other.uvCoordinate || normal != other.normal; }

		~Vertex() = default;
	};

	class Mesh : public Asset {
	public:
		Vertex* vertices{ nullptr }; size_t vertexC{};
		size_t* indices{ nullptr }; size_t indexC{};

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
			delete[] vertices;
			delete[] indices;
		}
	};
}