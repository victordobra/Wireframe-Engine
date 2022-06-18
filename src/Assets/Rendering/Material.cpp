#include "Material.hpp"

namespace mge {
    // External functions
    Material::Material(Shader* shader) : shader(shader) {
        shader->materials.insert_or_assign(this);

        // Calculate the total size
        size_t imageCount = 0;
        VkDeviceSize size = 0;
        for(const auto& property : shader->properties)
            switch(property.type) {
            case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE:
                // Increment the image counter
                ++imageCount;
                break;
            case Shader::ShaderProperty::SHADER_RPOPERTY_TYPE_COLOR:
            {
                // Align to a Vector4
                VkDeviceSize vectorCount = (size + sizeof(Vector4) - sizeof(float32_t)) / sizeof(Vector4);
                size = (vectorCount + 1) * sizeof(Vector4);
                break;
            }
            case Shader::ShaderProperty::SHADER_RPOPERTY_TYPE_FLOAT:
                size += sizeof(float32_t);
                break;
            }
        
        // Create every buffer
        if(size)
            for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
                buffers[i] = new Buffer(size, 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

        // Create every image
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            images[i].resize(imageCount);
            
            for(size_t j = 0; j < imageCount; ++j)
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

        // Calculate the total size
        size_t imageCount = 0;
        VkDeviceSize size = 0;
        for(const auto& property : shader->properties)
            switch(property.type) {
            case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE:
                // Increment the image counter
                ++imageCount;
                break;
            case Shader::ShaderProperty::SHADER_RPOPERTY_TYPE_COLOR:
            {
                // Align to a Vector4
                VkDeviceSize vectorCount = (size + sizeof(Vector4) - sizeof(float32_t)) / sizeof(Vector4);
                size = (vectorCount + 1) * sizeof(Vector4);
                break;
            }
            case Shader::ShaderProperty::SHADER_RPOPERTY_TYPE_FLOAT:
                size += sizeof(float32_t);
                break;
            }
        
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
            images[i].resize(imageCount);

        for(size_t i = 0; i < imageCount; ++i) {
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
        string shaderLocation = shader->location;
        uint64_t len64 = shaderLocation.length();
        output.WriteBuffer((char_t*)&len64, sizeof(uint64_t));
        output.WriteBuffer((char_t*)shaderLocation.c_str(), len64 * sizeof(char_t));

        // Calculate the total size
        size_t imageCount = 0;
        for(const auto& property : shader->properties)
            if(property.type == Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE)
                ++imageCount;

        VkDeviceSize size = (VkDeviceSize)((shader->properties.size() - imageCount) * sizeof(Vector4));

        // Save the data to the file
        buffers[0]->Map();
        output.WriteBuffer((char_t*)buffers[0]->GetMappedMemory(), size * sizeof(char_t));
        buffers[0]->Unmap();

        // Save every image's location to the file
        size_t imageIndex = 0;
        for(const auto& property : shader->properties) 
            if(property.type == Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE){
                len64 = (uint64_t)images[0][imageIndex]->location.size();
                output.WriteBuffer((char_t*)&len64, sizeof(uint64_t));
                output.WriteBuffer((char_t*)images[0][imageIndex++]->location.c_str(), len64 * sizeof(char_t));
            }

        output.Close();
    }

    Material::~Material() {
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            delete buffers[i];
    }

    MGE_ASSET(Material)
}