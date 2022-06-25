#pragma once
#include "Base/Asset.hpp"
#include "Shader.hpp"
#include "Assets/General/Image.hpp"
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
            // Calculate the offset and the image index
            size_t imageIndex = 0;
            VkDeviceSize offset = 0;
            size_t propertyIndex = 0;
            for(const auto& property : shader->properties) {
                if(propertyName == property.name)
                    break;
                switch(property.type) {
                case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE:
                    // Increment the image counter
                    ++imageIndex;
                    break;
                case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_COLOR:
                {
                    // Align to a Vector4
                    VkDeviceSize vectorCount = (offset + sizeof(Vector4) - sizeof(float32_t)) / sizeof(Vector4);
                    offset = (vectorCount + 1) * sizeof(Vector4);
                    break;
                }
                case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_FLOAT:
                    offset += sizeof(float32_t);
                    break;
                }
                ++propertyIndex;
            }

            if(propertyIndex == shader->properties.size())
                console::OutFatalError("Failed to find a shader property with the specified name!", 1);

            if(shader->properties[propertyIndex].type == Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE)
                return images[0].begin() + imageIndex;
            if(shader->properties[propertyIndex].type == Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE) {
                // Align to a Vector4
                VkDeviceSize vectorCount = (offset + sizeof(Vector4) - sizeof(float32_t)) / sizeof(Vector4);
                offset = vectorCount * sizeof(Vector4);
            }

            T value;
            buffers[0]->ReadFromBuffer(&value, sizeof(T), offset);
            return value;
        }
        template<class T>
        void SetPropertyValue(const string& propertyName, const T& newValue) {
            // Calculate the offset and the image index
            size_t imageIndex = 0;
            VkDeviceSize offset = 0;
            size_t propertyIndex = 0;
            for(const auto& property : shader->properties) {
                if(propertyName == property.name)
                    break;
                switch(property.type) {
                case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE:
                    // Increment the image counter
                    ++imageIndex;
                    break;
                case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_COLOR:
                {
                    // Align to a Vector4
                    VkDeviceSize vectorCount = (offset + sizeof(Vector4) - sizeof(float32_t)) / sizeof(Vector4);
                    offset = (vectorCount + 1) * sizeof(Vector4);
                    break;
                }
                case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_FLOAT:
                    offset += sizeof(float32_t);
                    break;
                }
                ++propertyIndex;
            }

            if(propertyIndex == shader->properties.size())
                console::OutFatalError("Failed to find a shader property with the specified name!", 1);
            
            assert(shader->properties[propertyIndex].type != Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE && "The specified property is an image!");

            if(shader->properties[propertyIndex].type == Shader::ShaderProperty::SHADER_PROPERTY_TYPE_COLOR) {
                // Align to a Vector4
                VkDeviceSize vectorCount = (offset + sizeof(Vector4) - sizeof(float32_t)) / sizeof(Vector4);
                offset = vectorCount * sizeof(Vector4);
            }
            
            // Apply this change to the first buffer
            buffers[0]->WriteToBuffer((void*)&newValue, sizeof(T), offset);
            buffers[0]->Flush();
        }
        void SetPropertyValue(const string& propertyName, Image* newValue) {
            // Calculate the offset and the image index
            size_t imageIndex = 0;
            VkDeviceSize offset = 0;
            size_t propertyIndex = 0;
            for(const auto& property : shader->properties) {
                if(propertyName == property.name)
                    break;
                switch(property.type) {
                case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE:
                    // Increment the image counter
                    ++imageIndex;
                    break;
                case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_COLOR:
                {
                    // Align to a Vector4
                    VkDeviceSize vectorCount = (offset + sizeof(Vector4) - sizeof(float32_t)) / sizeof(Vector4);
                    offset = (vectorCount + 1) * sizeof(Vector4);
                    break;
                }
                case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_FLOAT:
                    offset += sizeof(float32_t);
                    break;
                }
                ++propertyIndex;
            }

            if(propertyIndex == shader->properties.size())
                console::OutFatalError("Failed to find a shader property with the specified name!", 1);

            assert(shader->properties[propertyIndex].type == Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE && "The specified property is not an image!");

            // Set the first image and copy every other image
            for(size_t i = 1; i < MAX_FRAMES_IN_FLIGHT; ++i)
                delete images[i][imageIndex];
            
            images[0][imageIndex] = newValue;
            images[0][imageIndex]->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

            size_t imageWidth  = images[0][imageIndex]->GetWidth();
            size_t imageHeight = images[0][imageIndex]->GetHeight();
            VkImage srcImage = images[0][imageIndex]->GetImage();

            for(size_t i = 1; i < MAX_FRAMES_IN_FLIGHT; ++i) {
                images[i][imageIndex] = new Image(imageWidth, imageHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
                images[i][imageIndex]->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
                CopyImage(srcImage, images[i][imageIndex]->GetImage(), imageWidth, imageHeight, 1, 1);
                images[i][imageIndex]->TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            }

            images[0][imageIndex]->TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
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
        vector<Image*> GetImages() const {
            return images[GetCurrentFrame()];
        }
        vector<Image*>* GetImageVectors() const {
            return (vector<Image*>*)images;
        }

        ~Material();
    protected:
        void LoadFromFile(const string& fileLocation) override;
        void SaveToFile  (const string& fileLocation) override;
    private:
        Shader* shader;
        Buffer* buffers[MAX_FRAMES_IN_FLIGHT];
        vector<Image*> images[MAX_FRAMES_IN_FLIGHT];

        friend Shader;
    };
}