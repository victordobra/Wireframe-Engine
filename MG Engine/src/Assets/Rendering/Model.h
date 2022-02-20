#pragma once
#include "Asset.h"
#include "EngineMath.h"

namespace mge {
	class Model : public Asset {
	public:
		class Vertex {
		public:
			Vector3 position{};
			Vector2 uvCoordinate{};
			Vector3 normal{};

			Vertex() = default;
			Vertex(const Vertex&) = default;
			Vertex(Vertex&&) noexcept = default;
			Vertex(Vector3 position, Vector2 uvCoordinate, Vector3 normal) : position(position), uvCoordinate(uvCoordinate), normal(normal) { }

			Vertex& operator=(const Vertex&) = default;
			Vertex& operator=(Vertex&&) noexcept = default;

			bool operator==(const Vertex& other) const { return position == other.position && uvCoordinate == other.uvCoordinate && normal == other.normal; }
			bool operator==(Vertex&& other)      const { return position == other.position && uvCoordinate == other.uvCoordinate && normal == other.normal; }
			bool operator!=(const Vertex& other) const { return position != other.position || uvCoordinate != other.uvCoordinate || normal != other.normal; }
			bool operator!=(Vertex&& other)      const { return position != other.position || uvCoordinate != other.uvCoordinate || normal != other.normal; }

			~Vertex() = default;
		};

		Vertex* vertices{ nullptr }; size_t vertexC{};
		size_t* indices{ nullptr }; size_t indexC{};

		Model() = default;
		Model(const Model& other);
		Model(Model&& other) noexcept;
		Model(const char_t* fileLocation);

		Model& operator=(const Model& other);
		Model& operator=(Model&& other) noexcept;

		bool operator==(const Model&) const = delete;
		bool operator==(Model&&) const = delete;
		bool operator!=(const Model&) const = delete;
		bool operator!=(Model&&) const = delete;

		void SaveToFile(const char_t* fileLocation) const override;

		~Model() {
			delete[] vertices;
			delete[] indices;
		}
	};
}