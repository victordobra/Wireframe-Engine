#pragma once

#include "Base/Asset.hpp"
#include "Vulkan/VulkanInclude.hpp"
#include "Core.hpp"

namespace mge {
    class Pipeline;
    class Material;

    class Shader : public Asset {
    public:
        /// @brief Stores a property of the shader.
        struct ShaderProperty {
            string name;
            size_t size;
            size_t offset;
        };

        Shader() = default;
        Shader(const Shader&) = delete;
        Shader(Shader&&) noexcept = delete;
        Shader(const string& spirvPath);

        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&) noexcept = delete;

        void LoadFromFile(const string& filePath) override;
        void SaveToFile(const string& filePath) override;

        /// @brief Returns all of the shader's properties
        vector<ShaderProperty> GetProperties() const {
            vector<ShaderProperty> propertyVec((size_t)(properties.end() - properties.begin()));
            size_t ind = 0;
            for(const auto& property : properties)
                propertyVec[ind++] = property.val2;
            return propertyVec;
        }
        /// @brief Returns a map of all of the shader properties and their names.
        map<string, ShaderProperty> GetPropertyMap() const { return properties; }
        /// @brief Adds a new property to the shader.
        template<class T>
        void AddProperty(const string& propertyName) {
            ShaderProperty property{};
            
            if(properties.begin() == properties.end())
                property.offset = 0;
            else
                property.offset = (properties.end() - 1)->val2.offset + (properties.end() - 1)->val2.size;

            property.size = sizeof(T);
            property.name = propertyName;

            properties.insert(propertyName, property);
        }

        /// @brief Returns the shader's module.
        VkShaderModule GetModule() const { return shaderModule; }
        /// @brief Returns the shader's parent pipeline
        Pipeline* GetPipeline() const { return pipeline; }
        /// @brief Returns a vector of every material that has this shader.
        set<Material*> GetMaterials() const { return materials; }

        ~Shader();
    private:
        void LoadFromBinary(const string& spirvPath);

        string spirvPath;
        VkShaderModule shaderModule;

        map<string, ShaderProperty> properties;
        set<Material*> materials;
        Pipeline* pipeline;

        friend Pipeline;
        friend Material;
    };
}