#pragma once

#include "Base/Asset.hpp"
#include "Math/EngineMath.hpp"
#include "Vulkan/VulkanInclude.hpp"
#include "Vulkan/Buffer.hpp"

namespace wfe {
    class Model : public Asset {
    public:
        struct Vertex {
            Vector3 position;
            Vector2 uvCoord;
            Vector3 normal;
            Vector3 tangent;
            Vector3 bitangent;

            Vertex() = default;
            Vertex(const Vertex&) = default;
            Vertex(Vertex&&) noexcept = default;
            Vertex(Vector3 pos, Vector2 uv, Vector3 norm, Vector3 tan = {}) : position(pos), uvCoord(uv), normal(norm), tangent(tan) { }

            Vertex& operator=(const Vertex&) = default;
            Vertex& operator=(Vertex&&) noexcept = default;

            bool8_t operator==(const Vertex& other) const { 
                return position == other.position && uvCoord == other.uvCoord && normal == other.normal && tangent == other.tangent && bitangent == other.bitangent;
            } 
            bool8_t operator==(Vertex&& other)      const { 
                return position == other.position && uvCoord == other.uvCoord && normal == other.normal && tangent == other.tangent && bitangent == other.bitangent;
            } 
            bool8_t operator!=(const Vertex& other) const { 
                return position != other.position || uvCoord != other.uvCoord || normal != other.normal || tangent != other.tangent || bitangent != other.bitangent;
            }
			bool8_t operator!=(Vertex&& other)      const { 
                return position != other.position || uvCoord != other.uvCoord || normal != other.normal || tangent != other.tangent || bitangent != other.bitangent;
            }
            
            static vector<VkVertexInputBindingDescription>   GetBindingDescriptions();
			static vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

            ~Vertex() = default;
        };

        Model() = default;
        Model(const Model&) = delete;
        Model(Model&&) noexcept = delete;
        Model(const string& fileLocation);

        Model& operator=(const Model&) = delete;
        Model& operator=(Model&&) noexcept = delete;

              Buffer* GetVertexBuffer()       { return vertexBuffer; }
        const Buffer* GetVertexBuffer() const { return vertexBuffer; }
              Buffer* GetIndexBuffer()        { return indexBuffer;  }
        const Buffer* GetIndexBuffer()  const { return indexBuffer;  }

        size_t GetVertexCount() const { return (size_t)vertexCount; }
        size_t GetIndexCount()  const { return (size_t)indexCount;  }

        void Bind(VkCommandBuffer commandBuffer) const;
        void Draw(VkCommandBuffer commandBuffer) const;

        ~Model();
    protected:
        void LoadFromFile(const string& fileLocation) override;
        void SaveToFile  (const string& fileLocation) override;
    private:   
        void CreateVertexBuffer(const vector<Vertex>& vertices);
        void CreateIndexBuffer (const vector<uint32_t>& indices);

        Buffer* vertexBuffer;
        Buffer* indexBuffer;
        uint32_t vertexCount, indexCount;
    };
}