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
        input.ReadBuffer(data, size);

        input.Close();

        VkShaderModuleCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = size;
        createInfo.pCode = (uint32_t*)data;
        createInfo.flags = 0;
        createInfo.pNext = nullptr;

        auto result = vkCreateShaderModule(GetDevice(), &createInfo, nullptr, &shaderModule);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failes to create shader! Error code: " + VkResultToString(result), 1);
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
        input.ReadBuffer((char_t*)&propertyCount, sizeof(uint64_t));

        // Read every property
        for(size_t i = 0; i < propertyCount; i++) {
            ShaderProperty property;

            // Read the string
            uint64_t stringLength{};
            input.ReadBuffer((char_t*)&stringLength, sizeof(uint64_t));
            property.name.resize((size_t)stringLength);

            input.ReadBuffer((char_t*)property.name.c_str(), stringLength * sizeof(char_t));
            property.name[stringLength] = 0;

            // Read the type
            input.ReadBuffer((char_t*)&property.type, sizeof(ShaderProperty::ShaderPropertyType));

            // Insert the property into the map
            properties.push_back(property);
        }

        // Read the SPIR-V binary file location
        uint64_t stringLength{};
        input.ReadBuffer((char_t*)&stringLength, sizeof(uint64_t));
        spirvPath.resize((size_t)stringLength);

        input.ReadBuffer((char_t*)spirvPath.c_str(), stringLength * sizeof(char_t));
        spirvPath[stringLength] = 0;

        LoadFromBinary(spirvPath);

        // Read the pipeline location
        string pipelineLocation;
        input.ReadBuffer((char_t*)&stringLength, sizeof(uint64_t));
        pipelineLocation.resize((size_t)stringLength);

        input.ReadBuffer((char_t*)pipelineLocation.c_str(), stringLength * sizeof(char_t));
        pipelineLocation[stringLength] = 0;

        // Read the locations of every material
        uint64_t materialCount{};
        input.ReadBuffer((char_t*)&materialCount, sizeof(uint64_t));

        for(uint64_t i = 0; i < materialCount; ++i) {
            // Read the location of the material
            string materialLocation;
            uint64_t stringLength{};

            input.ReadBuffer((char_t*)&stringLength, sizeof(uint64_t));
            materialLocation.resize((size_t)stringLength);

            input.ReadBuffer((char_t*)materialLocation.c_str(), stringLength * sizeof(char_t));
            materialLocation[stringLength] = 0;

            // Load the material from the file
            Material* material = Asset::GetOrCreateAssetWithLocation<Material>(materialLocation);
            material->shader = this;
            materials.insert(material);
        }

        pipeline = Asset::GetOrCreateAssetWithLocation<Pipeline>(pipelineLocation);

        input.Close();
    }
    void Shader::SaveToFile(const string& filePath) {
        FileOutput output{filePath, StreamType::BINARY};

        if(!output)
            console::OutFatalError("Failed to open file!", 1);
        
        // Save all of the properties
        uint64_t propertyCount = (uint64_t)(properties.end() - properties.begin());
        output.WriteBuffer((char_t*)&propertyCount, sizeof(uint64_t));

        for(const auto& property : properties) {
            // Write the name
            uint64_t nameLength = (uint64_t)property.name.length();
            output.WriteBuffer((char_t*)&nameLength, sizeof(uint64_t));
            output.WriteBuffer((char_t*)property.name.c_str(), nameLength * sizeof(char_t));

            // Write the property type
            output.WriteBuffer((char_t*)&property.type, sizeof(ShaderProperty::ShaderPropertyType));
        }

        // Write the SPIR-V binary file location
        uint64_t stringLength = (uint64_t)spirvPath.length();
        output.WriteBuffer((char_t*)&stringLength, sizeof(uint64_t));
        output.WriteBuffer((char_t*)spirvPath.c_str(), stringLength * sizeof(char_t));

        // Write the pipeline location
        string str = "";
        if(pipeline)
            str = pipeline->location;

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