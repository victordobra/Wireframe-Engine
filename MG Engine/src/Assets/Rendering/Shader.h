#pragma once
#include "Asset.h"
#include "VulkanInclude.h"

#include <vector>

namespace mge {
	class Shader : public Asset {
	public:
		Shader() = delete;
		Shader(const Shader&) = delete;
		Shader(Shader&&) noexcept = delete;

		Shader(const char_t* fileLocation);

		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) noexcept = delete;

		VkShaderModule GetShaderModule() { return shaderModule; }

		void SaveToFile(const char_t* fileLocation) const override;

		~Shader();
	private:
		VkShaderModule shaderModule{};
	};
}