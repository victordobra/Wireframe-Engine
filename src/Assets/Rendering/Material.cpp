#include "Material.hpp"

namespace mge {
    // External functions
    Material::Material(Shader* shader) : shader(shader) {
        shader->materials.insert_or_assign(this);

        if(shader->properties.begin() != shader->properties.end()) {
            const auto* lastProp = shader->properties.end() - 1;
            VkDeviceSize size = (VkDeviceSize)(lastProp->val2.offset + lastProp->val2.size);
            
            for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
                buffers[i] = new Buffer(size, 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
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

        if(size)
            for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
                buffers[i] = new Buffer((VkDeviceSize)size, 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

                buffers[i]->Map();
                buffers[i]->WriteToBuffer(data);
                buffers[i]->Flush();
                buffers[i]->Unmap();
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

        output.WriteBuffer((char_t*)buffers[0]->GetMappedMemory(), size * sizeof(char_t));

        output.Close();
    }

    Material::~Material() {
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            delete buffers[i];
    }

    MGE_ASSET(Material)
}