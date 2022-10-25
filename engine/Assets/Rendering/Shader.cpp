#include "Shader.hpp"
#include "Vulkan/Device.hpp"
#include "WireframeEngineEditor.hpp"
#include "Pipeline.hpp"
#include "Material.hpp"

namespace wfe {
    // Internal helper functions
    void Shader::LoadFromBinary(const string& spirvLocation, bool8_t isFullLocation) {
        string fullLocation;
        if(!isFullLocation)
            if(editor::IsInsideEditor())
                fullLocation = editor::GetWorkspaceDir() + "assets/" + spirvLocation;
            else
                fullLocation = (string)WFE_ASSET_LOCATION + spirvLocation;
        else
            fullLocation = spirvLocation;

        FileInput input{fullLocation, (StreamType)(STREAM_TYPE_BINARY | STREAM_TYPE_AT_THE_END)};

        size_t size = input.Tell();
        input.Seek(0);
        char_t* data = new char_t[size];
        input.ReadBuffer(data, size);

        input.Close();

        VkShaderModuleCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = size;
        createInfo.pCode = (uint32_t*)data;
        createInfo.flags = 0;
        createInfo.pNext = nullptr;

        auto result = vkCreateShaderModule(GetDevice(), &createInfo, GetVulkanAllocator(), &shaderModule);
        if(result != VK_SUCCESS)
            console::OutFatalError((string)"Failes to create shader! Error code: " + VkResultToString(result), 1);
    }

    // External functions
    Shader::Shader(const string& spirvLocation) : spirvLocation(spirvLocation) {
        LoadFromBinary(spirvLocation);
    }

    void Shader::LoadFromFile(const string& fileLocation) {
        FileInput input{fileLocation, STREAM_TYPE_BINARY};

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
        spirvLocation.resize((size_t)stringLength);

        input.ReadBuffer((char_t*)spirvLocation.c_str(), stringLength * sizeof(char_t));
        spirvLocation[stringLength] = 0;

        LoadFromBinary(spirvLocation);

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
    void Shader::SaveToFile(const string& fileLocation) {
        FileOutput output{fileLocation, STREAM_TYPE_BINARY};

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
        uint64_t stringLength = (uint64_t)spirvLocation.length();
        output.WriteBuffer((char_t*)&stringLength, sizeof(uint64_t));
        output.WriteBuffer((char_t*)spirvLocation.c_str(), stringLength * sizeof(char_t));

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
    void Shader::ImportFromFile(const string& fileLocation) {
        LoadFromBinary(fileLocation, true);
    }

    void Shader::DrawEditorWindow() {
        for(auto& property : properties) {
            if(ImGui::Button("X")) {
                properties.erase(&property);

                for(auto* material : materials) {
                    vector<Vector4> colorValues;
                    vector<float32_t> floatValues;
                    vector<Image*> imageValues;

                    for(uint32_t i = 0; i < properties.size(); ++i) {
                        switch(properties[i].type) {
                        case ShaderProperty::SHADER_PROPERTY_TYPE_COLOR:
                            colorValues.push_back(material->GetPropertyValue<Vector4>(properties[i].name));
                            break;
                        case ShaderProperty::SHADER_PROPERTY_TYPE_FLOAT:
                            floatValues.push_back(material->GetPropertyValue<float32_t>(properties[i].name));
                            break;
                        case ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE:
                            imageValues.push_back(material->GetPropertyValue<Image*>(properties[i].name));
                            break;
                        }
                    }

                    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
                        delete material->buffers[i];
                        material->images[i].clear();
                    }
                    
                    material->CreateMaterial(this);

                    size_t colorTop = 0, floatTop = 0, imageTop = 0;

                    for(uint32_t i = 0; i < properties.size(); ++i) {
                        switch(properties[i].type) {
                        case ShaderProperty::SHADER_PROPERTY_TYPE_COLOR:
                            material->SetPropertyValue(properties[i].name, colorValues[colorTop++]);
                            break;
                        case ShaderProperty::SHADER_PROPERTY_TYPE_FLOAT:
                            material->SetPropertyValue(properties[i].name, floatValues[floatTop++]);
                            break;
                        case ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE:
                            material->SetPropertyValue(properties[i].name, imageValues[imageTop++]);
                            break;
                        }
                    }
                }
            }
            
            ImGui::SameLine();
            
            char_t buffer[256];
            strcpy(buffer, property.name.c_str());
        
            const char_t* label;
            switch(property.type) {
            case ShaderProperty::SHADER_PROPERTY_TYPE_COLOR:
                label = "Color";
                break;
            case ShaderProperty::SHADER_PROPERTY_TYPE_FLOAT:
                label = "Float";
                break;
            case ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE:
                label = "Image";
                break;
            }

            if(ImGui::InputText(label, buffer, 256)) {
                property.name = buffer;
            }

            ImGui::PushItemWidth(-.1f);

            bool8_t addColor = ImGui::Button("Add color");
            bool8_t addFloat = ImGui::Button("Add float");
            bool8_t addImage = ImGui::Button("Add image");
            if(addColor || addFloat || addImage) {
                if(addColor)
                    properties.push_back({ "New color", ShaderProperty::SHADER_PROPERTY_TYPE_COLOR });
                else if(addFloat)
                    properties.push_back({ "New float", ShaderProperty::SHADER_PROPERTY_TYPE_FLOAT });
                else if(addImage)
                    properties.push_back({ "New image", ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE });

                for(auto* material : materials) {
                    vector<Vector4> colorValues;
                    vector<float32_t> floatValues;
                    vector<Image*> imageValues;

                    for(uint32_t i = 0; i < properties.size() - 1; ++i) {
                        switch(properties[i].type) {
                        case ShaderProperty::SHADER_PROPERTY_TYPE_COLOR:
                            colorValues.push_back(material->GetPropertyValue<Vector4>(properties[i].name));
                            break;
                        case ShaderProperty::SHADER_PROPERTY_TYPE_FLOAT:
                            floatValues.push_back(material->GetPropertyValue<float32_t>(properties[i].name));
                            break;
                        case ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE:
                            imageValues.push_back(material->GetPropertyValue<Image*>(properties[i].name));
                            break;
                        }
                    }

                    if(addColor)
                        colorValues.push_back({ 0, 0, 0, 0 });
                    else if(addFloat)
                        floatValues.push_back(0);
                    else if(addImage)
                        imageValues.push_back(nullptr);

                    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
                        delete material->buffers[i];
                        material->images[i].clear();
                    }
                    
                    material->CreateMaterial(this);

                    size_t colorTop = 0, floatTop = 0, imageTop = 0;

                    for(uint32_t i = 0; i < properties.size(); ++i) {
                        switch(properties[i].type) {
                        case ShaderProperty::SHADER_PROPERTY_TYPE_COLOR:
                            material->SetPropertyValue(properties[i].name, colorValues[colorTop++]);
                            break;
                        case ShaderProperty::SHADER_PROPERTY_TYPE_FLOAT:
                            material->SetPropertyValue(properties[i].name, floatValues[floatTop++]);
                            break;
                        case ShaderProperty::SHADER_PROPERTY_TYPE_IMAGE:
                            material->SetPropertyValue(properties[i].name, imageValues[imageTop++]);
                            break;
                        }
                    }
                }
                
            }
        }
    }

    Shader::~Shader() {
        vkDeviceWaitIdle(GetDevice());
        vkDestroyShaderModule(GetDevice(), shaderModule, GetVulkanAllocator());
    }

    WFE_ASSET(Shader)
}