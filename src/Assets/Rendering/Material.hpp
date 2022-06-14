#pragma once
#include "Base/Asset.hpp"
#include "Shader.hpp"
#include "Vulkan/Buffer.hpp"
#include "Vulkan/SwapChain.hpp"

namespace mge {
    class Material : public Asset {
    public:
        Material() = default;
        Material(const Material&) = delete;
        Material(Material&&) noexcept = delete;
        Material(Shader* shader);

        Material& operator=(const Material&) = delete;
        Material& operator=(Material&&) noexcept = delete;

        void Map() {
            // Map the first buffer and only modify it
            buffers[0]->Map();
        }
        template<class T>
        T GetPropertyValue(const string& propertyName) {
            Shader::ShaderProperty property = shader->properties[propertyName];
            
            assert((property.size == sizeof(T)) && "Invalid property size!");

            T val;
            buffers[0]->ReadFromBuffer(&val, sizeof(T), property.offset);
            return val;
        }
        template<class T>
        void SetPropertyValue(const string& propertyName, const T& newValue) {
            Shader::ShaderProperty property = shader->properties[propertyName];
            
            assert((property.size == sizeof(T)) && "Invalid property size!");

            buffers[0]->WriteToBuffer((void*)&newValue, sizeof(T), property.offset);
        }
        void Unmap() {
            // Unmap the first buffer
            buffers[0]->Unmap();

            // Copy the first buffer to every buffer
            VkBuffer firstBuffer = buffers[0]->GetBuffer();
            VkDeviceSize bufferSize = buffers[0]->GetBufferSize();

            for(size_t i = 1; i < MAX_FRAMES_IN_FLIGHT; ++i)
                CopyBuffer(firstBuffer, buffers[i]->GetBuffer(), bufferSize);
        }

        Shader* GetShader() const { 
            return shader; 
        }
        Buffer* GetBuffer() const {
            return buffers[GetCurrentFrame()];
        }
        Buffer** GetBuffers() const {
            return (Buffer**)buffers;
        }

        ~Material();
    protected:
        void LoadFromFile(const string& fileLocation) override;
        void SaveToFile  (const string& fileLocation) override;
    private:
        Shader* shader;
        Buffer* buffers[MAX_FRAMES_IN_FLIGHT];
    };
}