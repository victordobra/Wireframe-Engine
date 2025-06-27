#include "Shader.hpp"
#include "Main/Program.hpp"
#include <vulkan/vk_enum_string_helper.h>

namespace wfe {
	// Virtual function definitions
	void Shader::Load(std::istream& stream) {
		// Read the size of the source code
		stream.seekg(0, std::ios::end);
		size_t newSourceSize = (size_t)stream.tellg();
		stream.seekg(0, std::ios::beg);

		if(newSourceSize & 3)
			throw std::runtime_error("Shader source size is not a multiple of 4 bytes!");

		// Destroy the shader's old data, if it exists
		if(shaderModule)
			GetProgram()->GetRenderer()->GetLoader()->vkDestroyShaderModule(GetProgram()->GetRenderer()->GetDevice()->GetDevice(), shaderModule, &VulkanRenderer::ALLOCATION_CALLBACKS);
		FreeMemory(source);

		// Allocate the new source code buffer
		sourceSize = newSourceSize;
		source = (uint8_t*)AllocMemory(sourceSize);
		if(!source)
			throw std::bad_alloc();
		
		// Read the source code from the stream
		if(stream.read((char*)source, sourceSize).gcount() != sourceSize)
			throw std::runtime_error("Failed to read shader source code from stream!");
		
		// Set the shader module info
		VkShaderModuleCreateInfo shaderModuleInfo {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.codeSize = sourceSize,
			.pCode = (const uint32_t*)source
		};

		// Create the shader module
		VkResult result = GetProgram()->GetRenderer()->GetLoader()->vkCreateShaderModule(GetProgram()->GetRenderer()->GetDevice()->GetDevice(), &shaderModuleInfo, &VulkanRenderer::ALLOCATION_CALLBACKS, &shaderModule);
		if(result != VK_SUCCESS)
			throw std::runtime_error((std::string)"Failed to create shader module! Error code: %s" + string_VkResult(result));
	}
	void Shader::Save(std::ostream& stream) const {
		// Write the source code to the stream
		if(!stream.write((const char*)source, sourceSize))
			throw std::runtime_error("Failed to write shader source code to stream!");
	}
	void Shader::Import(const std::string& path) {
		// Open the file stream
		std::ifstream stream(path, std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open shader file \"" + path + "\" for reading!");

		// Load the shader from the stream
		Load(stream);

		// Close the file stream
		stream.close();
	}
	void Shader::Export(const std::string& path) const {
		// Open the file stream
		std::ofstream stream(path, std::ios::binary);
		if(!stream)
			throw std::runtime_error("Failed to open shader file \"" + path + "\" for writing!");

		// Save the shader to the stream
		Save(stream);

		// Close the file stream
		stream.close();
	}

	Shader::~Shader() {
		// Destroy the shader module, if it exists
		if(shaderModule)
			GetProgram()->GetRenderer()->GetLoader()->vkDestroyShaderModule(GetProgram()->GetRenderer()->GetDevice()->GetDevice(), shaderModule, &VulkanRenderer::ALLOCATION_CALLBACKS);
		
		// Free the source code buffer
		FreeMemory(source);
	}
}