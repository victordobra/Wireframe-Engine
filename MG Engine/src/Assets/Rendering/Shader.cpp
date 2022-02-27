#include "Shader.h"
#include "VulkanDevice.h"
#include "Debugger.h"

#include <fstream>

namespace mge {
	Shader::Shader(const char_t* fileLocation) {
		std::ifstream fileInput(fileLocation, std::ios::binary | std::ios::ate);

		if (!fileInput)
			OutFatalError("Failed to open file");

		size_t fileSize = (size_t)fileInput.tellg();
		std::vector<char_t> code(fileSize);

		fileInput.seekg(0);
		fileInput.read(code.data(), fileSize);

		if (fileInput.bad())
			OutFatalError("Error occured while reading file!");

		fileInput.close();

		VkShaderModuleCreateInfo shaderModuleInfo{};
		shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleInfo.codeSize = (uint32_t)fileSize;
		shaderModuleInfo.pCode = (const ::uint32_t*)code.data();

		if (vkCreateShaderModule(GetDevice(), &shaderModuleInfo, nullptr, &shaderModule) != VK_SUCCESS)
			OutFatalError("Failed to create shader module!");
	}

	void Shader::SaveToFile(const char_t* fileLocation) const {
		OutFatalError("Cannot save a shader to a file!");
	}

	Shader::~Shader() {
		vkDestroyShaderModule(GetDevice(), shaderModule, nullptr);
	}
}