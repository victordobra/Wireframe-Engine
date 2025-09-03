#pragma once

#include "Core/Math/General/VecUtils.hpp"
#include "Core/Types/Defines.hpp"
#include "Vulkan/VulkanRenderer.hpp"

namespace wfe {
	/// @brief A mesh used for rendering.
	class RenderMesh {
	public:
		/// @brief A struct containing a singular vertex's info
		struct Vertex {
			/// @brief The vertex's position.
			Vec3f position;
			/// @brief The vertex's UV coordinates.
			Vec2f uvCoord;
			/// @brief The vertex's unit normal vector.
			Vec3f normal;
			/// @brief The vertex's unit tangent vector.
			Vec3f tangent;
		};

		/// @brief Creates a new render mesh.
		/// @param renderer The Vulkan renderer the mesh is created with.
		/// @param vertices The mesh's vertices.
		/// @param indices The mesh's vertex indices. Every 3 indices form a triangle.
		RenderMesh(VulkanRenderer* renderer, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		RenderMesh(const RenderMesh&) = delete;
		RenderMesh(RenderMesh&&) = delete;

		RenderMesh& operator=(const RenderMesh&) = delete;
		RenderMesh& operator=(RenderMesh&&) = delete;

		/// @brief Gets the Vulkan renderer of the mesh.
		/// @return The Vulkan renderer of the mesh.
		VulkanRenderer* GetRenderer() const {
			return renderer;
		}
		/// @brief Gets the number of vertices in the mesh.
		/// @return The number of vertices in the mesh.
		size_t GetVertexCount() const {
			return vertexCount;
		}
		/// @brief Gets the mesh's vertex buffer.
		/// @return The mesh's vertex buffer.
		VkBuffer GetVertexBuffer() const {
			return vertexBuffer;
		}
		/// @brief Gets the mesh vertex buffer's memory.
		/// @return The mesh vertex buffer's memory.
		VulkanAllocator::Memory GetVertexBufferMemory() const {
			return vertexBufferMemory;
		}
		/// @brief Gets the mesh's index buffer.
		/// @return The mesh's index buffer.
		size_t GetIndexCount() const {
			return indexCount;
		}
		/// @brief Gets the mesh's index buffer.
		/// @return The mesh's index buffer.
		VkBuffer GetIndexBuffer() const {
			return indexBuffer;
		}
		/// @brief Gets the mesh index buffer's memory.
		/// @return The mesh index buffer's memory.
		VulkanAllocator::Memory GetIndexBufferMemory() const {
			return indexBufferMemory;
		}
		/// @brief Gets the mesh's vertices and indices.
		/// @param vertices A pointer to the array in which all vertex data will be written.
		/// @param indices A pointer to the array in which all indices will be written.
		void GetMeshData(Vertex* vertices, uint32_t* indices) const;

		/// @brief Destroys the render mesh.
		~RenderMesh();
	private:
		VulkanRenderer* renderer;

		size_t vertexCount;
		VkBuffer vertexBuffer;
		VulkanAllocator::Memory vertexBufferMemory;
		size_t indexCount;
		VkBuffer indexBuffer;
		VulkanAllocator::Memory indexBufferMemory;
	};
}