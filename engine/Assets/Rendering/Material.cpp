#include "Material.hpp"
#include "WireframeEngineEditor.hpp"

namespace wfe {
    // Internal helper functions
    void Material::CreateMaterial(Shader* shader) {
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
            case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_COLOR:
            {
                // Align to a Vector4
                VkDeviceSize vectorCount = (size + sizeof(Vector4) - sizeof(float32_t)) / sizeof(Vector4);
                size = (vectorCount + 1) * sizeof(Vector4);
                break;
            }
            case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_FLOAT:
                size += sizeof(float32_t);
                break;
            }
        
        // Create every buffer
        if(size) {
            for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
                buffers[i] = new Buffer(size, 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            Map();
            memset(buffers[0]->GetMappedMemory(), 0, size);
            Unmap();
        }

        // Create every image
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            images[i].resize(imageCount);
            
            for(size_t j = 0; j < imageCount; ++j)
                images[i][j] = nullptr;
        }
    }

    // External functions
    Material::Material(Shader* shader) : shader(shader) {
        CreateMaterial(shader);
    }

    void Material::LoadFromFile(const string& fileLocation) {
        FileInput input(fileLocation, STREAM_TYPE_BINARY);

        if(input.IsAtTheEnd())
            return;

        // Load the shader location from the file
        string shaderLocation;
        uint64_t len64;
        input.ReadBuffer((char_t*)&len64, sizeof(uint64_t));

        if(!len64) {
            input.Close();
            return;
        }

        shaderLocation.resize((size_t)len64);
        input.ReadBuffer((char_t*)shaderLocation.c_str(), len64 * sizeof(char_t));
        shaderLocation[len64] = 0;

        // Calculate the total size
        uint64_t imageCount = 0;
        VkDeviceSize size = 0;
        
        input.ReadBuffer((char_t*)&size, sizeof(VkDeviceSize));
        char_t* data = new char_t[size];
        input.ReadBuffer(data, size * sizeof(char_t));

        // Create every buffer
        if(size)
            for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
                buffers[i] = new Buffer(size, 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

                buffers[i]->Map();
                buffers[i]->WriteToBuffer(data);
                buffers[i]->Flush();
                buffers[i]->Unmap();
            }
        
        // Load every image
        input.ReadBuffer((char_t*)&imageCount, sizeof(uint64_t));
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            images[i].resize(imageCount);

        // Begin single time commands
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        for(size_t i = 0; i < imageCount; ++i) {
            // Load the image's file location
            input.ReadBuffer((char_t*)&len64, sizeof(uint64_t));
            if(!len64)
                continue;

            string imageLocation{};
            imageLocation.resize(len64);

            input.ReadBuffer((char_t*)imageLocation.c_str(), len64 * sizeof(char_t));
            imageLocation[len64] = 0;

            // Load the first image
            images[0][i] = Asset::GetOrCreateAssetWithLocation<Image>(imageLocation);
            images[0][i]->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, commandBuffer);

            size_t imageWidth  = images[0][i]->GetWidth();
            size_t imageHeight = images[0][i]->GetHeight();
            VkImage srcImage = images[0][i]->GetImage();

            // Copy every other image
            for(size_t j = 1; j < MAX_FRAMES_IN_FLIGHT; ++j) {
                images[j][i] = new Image(imageWidth, imageHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
                images[j][i]->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, commandBuffer);
                CopyImage(srcImage, images[j][i]->GetImage(), imageWidth, imageHeight, 1, 1, commandBuffer);
                images[j][i]->TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, commandBuffer);
            }

            images[0][i]->TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, commandBuffer);
        }

        // Load the shader
        shader = Asset::GetOrCreateAssetWithLocation<Shader>(shaderLocation);
        shader->materials.insert(this);

        input.Close();
    }
    void Material::SaveToFile(const string& fileLocation) {
        FileOutput output(fileLocation, STREAM_TYPE_BINARY);

        if(!shader) {
            const uint64_t zero = 0;
            output.WriteBuffer((char_t*)&zero, sizeof(uint64_t));
            output.Close();

            return;
        }

        Unmap();

        // Save the shader's location to the file
        string shaderLocation = shader->location;
        uint64_t len64 = shaderLocation.length();
        output.WriteBuffer((char_t*)&len64, sizeof(uint64_t));
        output.WriteBuffer((char_t*)shaderLocation.c_str(), len64 * sizeof(char_t));

        uint64_t imageCount = 0;
        VkDeviceSize size = 0;
        for(const auto& property : shader->properties)
            switch(property.type) {
            case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE:
                // Increment the image counter
                ++imageCount;
                break;
            case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_COLOR:
            {
                // Align to a Vector4
                VkDeviceSize vectorCount = (size + sizeof(Vector4) - sizeof(float32_t)) / sizeof(Vector4);
                size = (vectorCount + 1) * sizeof(Vector4);
                break;
            }
            case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_FLOAT:
                size += sizeof(float32_t);
                break;
            }

        // Save the data to the file
        output.WriteBuffer((char_t*)&size, sizeof(VkDeviceSize));
        buffers[0]->Map();
        output.WriteBuffer((char_t*)buffers[0]->GetMappedMemory(), size * sizeof(char_t));
        buffers[0]->Unmap();

        // Save every image's location to the file
        output.WriteBuffer((char_t*)&imageCount, sizeof(uint64_t));
        uint64_t imageIndex = 0;
        for(const auto& property : shader->properties) 
            if(property.type == Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE) {
                len64 = (uint64_t)images[0][imageIndex]->location.size();
                output.WriteBuffer((char_t*)&len64, sizeof(uint64_t));
                if(len64)
                    output.WriteBuffer((char_t*)images[0][imageIndex++]->location.c_str(), len64 * sizeof(char_t));
            }

        output.Close();
    }

    void Material::DrawEditorWindow() {
        string shaderName;
        if(shader)
            shaderName = shader->name;
        else
            shaderName = "None";

        auto& shaderType = Asset::assetTypes[WFE_TYPE_ID(Shader).Hash64()];
        if(ImGui::BeginCombo("Shader", shaderName.c_str())) {
            for(auto* asset : Asset::GetAssets()) {
                if(asset->assetType == &shaderType && ImGui::Selectable(asset->name.c_str(), shader == asset)) {
                    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
                        delete buffers[i];
                    
                    shader = (Shader*)asset;
                    if(shader)
                        CreateMaterial(shader);
                }
            }

            ImGui::EndCombo();
        }

        if(!shader)
            return;
        
        if(!buffers[0]->GetMappedMemory())
            Map();

        for(auto& property : shader->properties) {
            switch(property.type) {
            case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE:
            {
                Image* image = GetPropertyValue<Image*>(property.name);
                auto& imageType = Asset::assetTypes[WFE_TYPE_ID(Image).Hash64()];

                string imageName;
                if(image)
                    imageName = image->name;
                else
                    imageName = "None";
                
                if(ImGui::BeginCombo(property.name.c_str(), imageName.c_str())) {
                    for(auto* asset : Asset::GetAssets()) {
                        if(asset->assetType == &imageType && ImGui::Selectable(asset->name.c_str(), image == asset)) {
                            image = (Image*)asset;
                            SetPropertyValue(property.name, image);
                        }
                    }

                    ImGui::EndCombo();
                }

                break;
            }
            case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_COLOR:
            {
                Vector4 color = GetPropertyValue<Vector4>(property.name);

                ImGui::ColorEdit4(property.name.c_str(), color.elements, ImGuiColorEditFlags_Float);
                SetPropertyValue(property.name, color);

                break;
            }
            case Shader::ShaderProperty::SHADER_PROPERTY_TYPE_FLOAT:
            {
                float32_t number = GetPropertyValue<float32_t>(property.name);

                ImGui::InputFloat(property.name.c_str(), &number);
                SetPropertyValue(property.name, number);

                break;
            }
            }
        }
    }

    Material::~Material() {
        if(shader)
            shader->materials.erase(this);

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            delete buffers[i];
    }

    WFE_ASSET(Material)
}