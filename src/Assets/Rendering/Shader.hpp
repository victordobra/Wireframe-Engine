#pragma once

#include "Core/Assets/Asset.hpp"
#include "Core/Types/Defines.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief A class representing a shader module asset.
	class Shader : public Asset {
	public:
		/// @brief Creates a new blank shader. The shader must be subsequently loaded or imported.
		/// @param program The program that owns the asset.
		/// @param id The asset's ID, or UINT64_T_MAX if the asset has no ID.
		Shader(Program* program, uint64_t id = UINT64_T_MAX) : Asset(program, id) { }
		/// @brief Creates a new shader.
		/// @param program The program that owns the asset.
		/// @param sourceSize The size of the shader's SPIR-V source code, in bytes.
		/// @param source The shader's SPIR-V source code.
		/// @param id The asset's ID, or UINT64_T_MAX if the asset has no ID.
		Shader(Program* program, size_t sourceSize, const uint8_t* source, uint64_t id = UINT64_T_MAX);
		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;

		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) = delete;

		/// @brief Loads the shader from a binary stream, using the final encoding.
		/// @param stream The stream to load the shader from.
		void Load(std::istream& stream) override;
		/// @brief Saves the shader to a binary stream, using the final encoding.
		/// @param stream The stream to save the shader to.
		void Save(std::ostream& stream) const override;
		/// @brief Imports the shader from a file.
		/// @param path The path to the file to import the shader from.
		void Import(const std::string& path) override;
		/// @brief Exports the shader to a file.
		/// @param path The path to the file to export the shader to.
		void Export(const std::string& path) const override;
		/// @brief Gets the shader's dependencies.
		/// @return The shader's dependencies.
		std::vector<Asset*> GetDependencies() const override {
			return {};
		}

		/// @brief Gets the size of the shader's SPIR-V source code.
		/// @return The size of the shader's SPIR-V source code.
		size_t GetSourceSize() const {
			return sourceSize;
		}
		/// @brief Gets the shader's SPIR-V source code.
		/// @return The size of the shader's SPIR-V source code.
		const uint8_t* GetSource() const {
			return source;
		}
		/// @brief Gets the internal Vulkan shader module's handle.
		/// @return The internal Vulkan shader module's handle.
		VkShaderModule GetShaderModule() const {
			return shaderModule;
		}

		/// @brief Destroys the shader.
		~Shader();
	private:
		WFE_ASSET_TYPE(Shader, { "spv" })

		size_t sourceSize = 0;
		uint8_t* source = nullptr;
		VkShaderModule shaderModule = VK_NULL_HANDLE;
	};
}