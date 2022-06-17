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

        Image* GetImagePropertyValue(const string& propertyName) {
            size_t index = (size_t)(&shader->imageProperties[propertyName] - shader->imageProperties.begin());

            return images[0][index];
        }
        void SetImagePropertyValue(const string& propertyName, Image* newImage) {
            size_t index = (size_t)(&shader->imageProperties[propertyName] - shader->imageProperties.begin());

            images[0][index] = newImage;
            newImage->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

            size_t imageWidth  = images[0][index]->GetWidth();
            size_t imageHeight = images[0][index]->GetHeight();
            VkImage srcImage = images[0][index]->GetImage();

            // Copy every other image
            for(size_t i = 1; i < MAX_FRAMES_IN_FLIGHT; ++i) {
                delete images[i][index];

                images[i][index] = new Image(imageWidth, imageHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
                images[i][index]->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
                CopyImage(srcImage, images[i][index]->GetImage(), imageWidth, imageHeight, 1, 1);
                images[i][index]->TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            }

            images[0][index]->TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
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
    };
}