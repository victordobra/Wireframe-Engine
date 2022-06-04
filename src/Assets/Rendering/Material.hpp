#pragma once
#include "Base/Asset.hpp"
#include "Shader.hpp"

namespace mge {
    class Material : public Asset {
    public:
        Material() = default;
        Material(const Material&) = delete;
        Material(Material&&) noexcept = delete;
        Material(Shader* shader) : shader(shader) { 
            if(shader->properties.begin() != shader->properties.end()) {
                const auto* lastProp = shader->properties.end() - 1;
                size_t size = lastProp->val2.offset + lastProp->val2.size;
                data.resize(size);
            }
        }

        Material& operator=(const Material&) = delete;
        Material& operator=(Material&&) noexcept = delete;

        template<class T>
        T GetPropertyValue(const string& propertyName) {
            Shader::ShaderProperty property = shader->properties[propertyName];
            
            assert((property.size == sizeof(T)) && "Invalid property size!");

            char_t* ptr = data.data() + property.offset;
            T val;
            memcpy(&val, ptr, property.size);
            return val;
        }
        template<class T>
        void SetPropertyValue(const string& propertyName, const T& newValue) {
            Shader::ShaderProperty property = shader->properties[propertyName];
            
            assert((property.size == sizeof(T)) && "Invalid property size!");

            char_t* ptr = data.data() + property.offset;
            memcpy(ptr, &newValue, property.size);
        }

        Shader* GetShader() const { return shader; }
        vector<char_t> GetData() const { return data; }

        void LoadFromFile(const string& fileLocation) override;
        void SaveToFile(const string& fileLocation) override;

        ~Material() = default;
    private:
        Shader* shader;
        vector<char_t> data;
    };
}