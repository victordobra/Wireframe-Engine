#pragma once
#include "VulkanDevice.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Model.h"
#include "Buffer.h"
#include "Defines.h"

#include <vector>

namespace mge {
	class VulkanModel {
	public:
		struct Vertex {
			Vector3 position{};
			Vector3 color{};
			Vector3 normal{};
			Vector2 uv{};

			Vertex() = default;
			Vertex(const Vertex&) = default;
			Vertex(Vertex&&) noexcept = default;
			Vertex(const Model::Vertex& other) : position(other.position), color({1.0f, 1.0f, 1.0f}) /*Placeholder*/, normal(other.normal), uv(other.uvCoordinate) { }
			Vertex(Vector3 position, Vector3 color, Vector3 normal, Vector2 uv) : position(position), color(color), normal(normal), uv(uv) { }

			Vertex& operator=(const Vertex&) = default;
			Vertex& operator=(Vertex&&) noexcept = default;

			bool8_t operator==(const Vertex& other) const { return position == other.position && color == other.color && normal == other.normal && uv == other.uv; }
			bool8_t operator==(Vertex& other) { return position == other.position && color == other.color && normal == other.normal && uv == other.uv; }
			bool8_t operator!=(const Vertex& other) { return position != other.position || color != other.color || normal != other.normal || uv != other.uv; }
			bool8_t operator!=(Vertex& other) { return position != other.position || color != other.color || normal != other.normal || uv != other.uv; }

			static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

			~Vertex() = default;
		};

		VulkanModel(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		VulkanModel(const Model* model);

		VulkanModel(const VulkanModel&) = delete;
		VulkanModel(VulkanModel&&) noexcept = delete;

		VulkanModel& operator=(const VulkanModel&) = delete;
		VulkanModel& operator=(VulkanModel&&) noexcept = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

		~VulkanModel();
	private:
		void CreateVertexBuffer(const std::vector<Vertex>& vertices);
		void CreateIndexBuffer(const std::vector<uint32_t>& indices);

		Buffer* vertexBuffer;
		uint32_t vertexCount;

		Buffer* indexBuffer;
		uint32_t indexCount;
	};
}