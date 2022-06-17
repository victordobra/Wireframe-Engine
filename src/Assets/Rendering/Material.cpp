#include "Material.hpp"

namespace mge {
    // External functions
    Material::Material(Shader* shader) : shader(shader) {
        shader->materials.insert_or_assign(this);

        if(shader->properties.begin() != shader->properties.end()) {
            // Calculate the total size
            VkDeviceSize size = 0;
            for(size_t i = 0; i < shader->properties.size(); ++i)
                size += (shader->properties.begin() + i)->val2.size;
            
            // Create every image
            for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
                buffers[i] = new Buffer(size, 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        }

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            images[i].resize(shader->imageProperties.size());
            
            for(size_t j = 0; j < shader->imageProperties.size(); ++j)
                images[i][j] = nullptr;
        }
    }

    void Material::LoadFromFile(const string& fileLocation) {
        FileInput input(fileLocation, StreamType::BINARY);

        // Load the shader location from the file
        string shaderLocation;
        uint64_t len64;
        input.Get((char_t*)&len64, sizeof(uint64_t));
        shaderLocation.resize((size_t)len64);
        input.Get((char_t*)shaderLocation.c_str(), len64 * sizeof(char_t));
        shaderLocation[len64] = 0;

        // Load the shader
        shader = Asset::GetOrCreateAssetWithLocation<Shader>(shaderLocation);
        shader->materials.insert_or_assign(this);

        // Load the material's data
        const auto* lastProp = shader->properties.end() - 1;
        size_t size = lastProp->val2.offset + lastProp->val2.size;
        
        char_t* data = new char_t[size];
        input.Get(data, size * sizeof(char_t));

        // Create every buffer
        if(size)
            for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
                buffers[i] = new Buffer((VkDeviceSize)size, 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

                buffers[i]->Map();
                buffers[i]->WriteToBuffer(data);
                buffers[i]->Flush();
                buffers[i]->Unmap();
            }
        
        // Load every image
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            images[i].resize(shader->imageProperties.size());

        for(size_t i = 0; i < shader->imageProperties.size(); ++i) {
            // Load the image's file path
            input.Get((char_t*)&len64, sizeof(uint64_t));
            string imageLocation{};
            imageLocation.resize(len64);

            input.Get((char_t*)imageLocation.c_str(), len64 * sizeof(char_t));
            imageLocation[len64] = 0;

            // Load the first image
            images[0][i] = Asset::GetOrCreateAssetWithLocation<Image>(imageLocation);
            images[0][i]->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

            size_t imageWidth  = images[0][i]->GetWidth();
            size_t imageHeight = images[0][i]->GetHeight();
            VkImage srcImage = images[0][i]->GetImage();

            // Copy every other image
            for(size_t j = 1; j < MAX_FRAMES_IN_FLIGHT; ++j) {
                images[j][i] = new Image(imageWidth, imageHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
                images[j][i]->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
                CopyImage(srcImage, images[j][i]->GetImage(), imageWidth, imageHeight, 1, 1);
                images[j][i]->TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            }

            images[0][i]->TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }

        input.Close();
    }
    void Material::SaveToFile(const string& fileLocation) {
        FileOutput output(fileLocation, StreamType::BINARY);

        // Save the shader's location to the file
        auto shaderLocation = shader->location;
        uint64_t len64 = shaderLocation.length();
        output.WriteBuffer((char_t*)&len64, sizeof(uint64_t));
        output.WriteBuffer((char_t*)shaderLocation.c_str(), len64 * sizeof(char_t));

        // Save the data to the file
        const auto* lastProp = shader->properties.end() - 1;
        size_t size = lastProp->val2.offset + lastProp->val2.size;

        buffers[0]->Map();
        output.WriteBuffer((char_t*)buffers[0]->GetMappedMemory(), size * sizeof(char_t));
        buffers[0]->Unmap();

        // Save every image's location to the file
        for(size_t i = 0; i < shader->imageProperties.size(); ++i) {
            len64 = (uint64_t)images[0][i]->location.size();
            output.WriteBuffer((char_t*)&len64, sizeof(uint64_t));
            output.WriteBuffer((char_t*)images[0][i]->location.c_str(), len64 * sizeof(char_t));
        }

        output.Close();
    }

    Material::~Material() {
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            delete buffers[i];
    }

    MGE_ASSET(Material)
}