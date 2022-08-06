#pragma once

#include "Base/Asset.hpp"
#include "Vulkan/VulkanInclude.hpp"
#include "Core.hpp"

namespace wfe {
    class Pipeline;
    class Material;

    class Shader : public Asset {
    public:
        /// @brief Stores a property of the shader.
        struct ShaderProperty {
            typedef enum : uint8_t {
                SHADER_PROPERTY_TYPE_COLOR,
                SHADER_PROPERTY_TYPE_FLOAT,
                SHADER_PROPERTY_TYPE_IMAGE
            } ShaderPropertyType;

            string name;
            ShaderPropertyType type;
        };

        Shader() = default;
        Shader(const Shader&) = delete;
        Shader(Shader&&) noexcept = delete;
        Shader(const string& spirvPath);

        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&) noexcept = delete;

        /// @brief Returns all of the shader's properties
        vector<ShaderProperty> GetProperties() const {
            return properties;
        }
        /// @brief Adds a new property to the shader.
        void AddProperty(const ShaderProperty& property) {
            properties.push_back(property);
        }

        /// @brief Returns the shader's module.
        VkShaderModule GetModule() const { 
            return shaderModule; 
        }
        /// @brief Returns the shader's parent pipeline
        Pipeline* GetPipeline() const { 
            return pipeline; 
        }
        /// @brief Returns a vector of every material that has this shader.
        set<Material*> GetMaterials() const { 
            return materials; 
        }

        ~Shader();
    protected:
        void LoadFromFile(const string& filePath) override;
        void SaveToFile(const string& filePath) override;
    private:
        void LoadFromBinary(const string& spirvPath);

        string spirvPath;
        VkShaderModule shaderModule;

        vector<ShaderProperty> properties;
        
        set<Material*> materials;
        Pipeline* pipeline{nullptr};

        friend Pipeline;
        friend Material;
    };
}