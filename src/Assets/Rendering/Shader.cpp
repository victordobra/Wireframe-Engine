#include "Shader.hpp"
#include "Vulkan/Device.hpp"
#include "Pipeline.hpp"
#include "Material.hpp"

namespace mge {
    // Internal helper functions
    void Shader::LoadFromBinary(const string& spirvPath) {
        FileInput input{(string)ASSET_PATH + spirvPath, StreamType::BINARY | StreamType::AT_THE_END};

        size_t size = input.Tell();
        input.Seek(0);
        char_t* data = (char_t*)malloc(size);
        input.Get(data, size);

        input.Close();

        VkShaderModuleCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = size;
        createInfo.pCode = (uint32_t*)data;
        createInfo.flags = 0;
        createInfo.pNext = nullptr;

        if(vkCreateShaderModule(GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
            console::OutFatalError("Failes to create shader!", 1);
    }

    // External functions
    Shader::Shader(const string& spirvPath) : spirvPath(spirvPath) {
        LoadFromBinary(spirvPath);
    }

    void Shader::LoadFromFile(const string& filePath) {
        FileInput input{filePath, StreamType::BINARY};

        if(!input)
            console::OutFatalError("Failed to open file!", 1);

        // Read the number of properties
        uint64_t propertyCount{};
        input.Get((char_t*)&propertyCount, sizeof(uint64_t));

        // Read every property
        for(size_t i = 0; i < propertyCount; i++) {
            ShaderProperty property;

            // Read the string
            uint64_t stringLength{};
            input.Get((char_t*)&stringLength, sizeof(uint64_t));
            property.name.resize((size_t)stringLength);

            input.Get((char_t*)property.name.c_str(), stringLength * sizeof(char_t));
            property.name[stringLength] = 0;

            // Read the other properties
            uint64_t size64{}, offset64{};
            
            input.Get((char_t*)&size64, sizeof(uint64_t));
            input.Get((char_t*)&offset64, sizeof(uint64_t));

            property.size = (size_t)size64;
            property.offset = (size_t)offset64;

            // Insert the property into the map
            properties.insert(property.name, property);
        }

        // Read the SPIR-V binary file location
        uint64_t stringLength{};
        input.Get((char_t*)&stringLength, sizeof(uint64_t));
        spirvPath.resize((size_t)stringLength);

        input.Get((char_t*)spirvPath.c_str(), stringLength * sizeof(char_t));
        spirvPath[stringLength] = 0;

        // Read the pipeline location
        string pipelinePath;
        input.Get((char_t*)&stringLength, sizeof(uint64_t));
        pipelinePath.resize((size_t)stringLength);

        input.Get((char_t*)pipelinePath.c_str(), stringLength * sizeof(char_t));
        pipelinePath[stringLength] = 0;

        // Read the locations of every material
        uint64_t materialCount{};
        input.Get((char_t*)&materialCount, materialCount);

        for(uint64_t i = 0; i < materialCount; ++i) {
            // Read the location of the material
            string materialLocation;
            uint64_t stringLength{};

            input.Get((char_t*)&stringLength, sizeof(uint64_t));
            materialLocation.resize((size_t)stringLength);

            input.Get((char_t*)materialLocation.c_str(), stringLength * sizeof(char_t));
            materialLocation[stringLength] = 0;

            // Load the material from the file
            materials.insert(Asset::GetOrCreateAssetWithLocation<Material>(materialLocation));
        }

        // Create the shader module
        LoadFromBinary(spirvPath);

        input.Close();
    }
    void Shader::SaveToFile(const string& filePath) {
        FileOutput output{filePath, StreamType::BINARY};

        if(!output)
            console::OutFatalError("Failed to open file!", 1);
        
        // Save all of the properties
        uint64_t propertyCount = (uint64_t)(properties.end() - properties.begin());
        output.WriteBuffer((char_t*)&propertyCount, sizeof(uint64_t));
        
        // Write every property
        for(const auto& property : properties) {
            // Write the string
            uint64_t stringLength = (uint64_t)property.val2.name.length();
            output.WriteBuffer((char_t*)&stringLength, sizeof(uint64_t));
            output.WriteBuffer((char_t*)property.val2.name.c_str(), stringLength * sizeof(char_t));

            // Write the other properties
            uint64_t size64 = (uint64_t)property.val2.size;
            uint64_t offset64 = (uint64_t)property.val2.offset;

            output.WriteBuffer((char_t*)&size64, sizeof(uint64_t));
            output.WriteBuffer((char_t*)&offset64, sizeof(uint64_t));
        }

        // Write the SPIR-V binary file location
        uint64_t stringLength = (uint64_t)spirvPath.length();
        output.WriteBuffer((char_t*)&stringLength, sizeof(uint64_t));
        output.WriteBuffer((char_t*)spirvPath.c_str(), stringLength * sizeof(char_t));

        // Write the pipeline location
        string str = "";
        stringLength = (uint64_t)str.length();
        output.WriteBuffer((char_t*)&stringLength, sizeof(uint64_t));
        output.WriteBuffer((char_t*)str.c_str(), stringLength * sizeof(char_t));

        // Write the locations of every material
        uint64_t materialCount = (uint64_t)(materials.end() - materials.begin());
        output.WriteBuffer((char_t*)&materialCount, sizeof(uint64_t));

        for(auto material : materials) {
            string materialLocation = material->location;

            // Write the material's path
            uint64_t stringLength = (uint64_t)materialLocation.length();
            output.WriteBuffer((char_t*)&stringLength, sizeof(uint64_t));
            output.WriteBuffer((char_t*)materialLocation.c_str(), stringLength * sizeof(char_t));
        }

        output.Close();
    }

    Shader::~Shader() {
        vkDeviceWaitIdle(GetDevice());
        vkDestroyShaderModule(GetDevice(), shaderModule, nullptr);
    }

    MGE_ASSET(Shader)
}