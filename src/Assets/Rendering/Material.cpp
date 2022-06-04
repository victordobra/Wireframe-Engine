#include "Material.hpp"

namespace mge {
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
        shader = (Shader*)Asset::LoadAssetFromFile<Shader>(shaderLocation);

        // Load the data from the file
        uint64_t size64;
        input.Get((char_t*)&size64, sizeof(uint64_t));
        data.resize(size64);
        input.Get(data.data(), size64 * sizeof(char_t));

        input.Close();
    }
    void Material::SaveToFile(const string& fileLocation) {
        FileOutput output(fileLocation, StreamType::BINARY);

        // Save the shader's location to the file
        auto shaderLocation = Asset::GetAssetLocation(shader);
        uint64_t len64 = shaderLocation.length();
        output.WriteBuffer((char_t*)&len64, sizeof(uint64_t));
        output.WriteBuffer((char_t*)shaderLocation.c_str(), len64 * sizeof(char_t));

        // Save the data to the file
        uint64_t size64 = data.size();
        output.WriteBuffer((char_t*)&size64, sizeof(uint64_t));
        output.WriteBuffer((char_t*)data.data(), size64 * sizeof(char_t));

        output.Close();
    }

    MGE_ASSET(Material)
}