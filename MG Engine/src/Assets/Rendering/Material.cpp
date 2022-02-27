#include "Material.h"
#include "Debugger.h"
#include "DescriptorPool.h"
#include "SwapChain.h"

#include <string>
#include <fstream>
#include <sstream>

namespace mge {
	std::vector<Material*> Material::materials = {};

	Material::Material() {
		materialIndex = materials.size();
		materials.push_back(this);
	}
	Material::Material(const Material& other) : color(other.color) {
		materialIndex = materials.size();
		materials.push_back(this);
	}
	Material::Material(Material&& other) noexcept : color(other.color) {
		materialIndex = materials.size();
		materials.push_back(this);
	}
	Material::Material(const char_t* fileLocation) {
		materialIndex = materials.size();
		materials.push_back(this);

		std::ifstream fileInput(fileLocation);

		if (!fileInput)
			OutFatalError("File not found!");

		std::string line;
		while (!fileInput.eof()) {
			std::getline(fileInput, line);
			if (line[0] == '#')
				return;

			std::stringstream sStream(line);

			std::string init;
			sStream >> init;

			if (init == "Ka" || init == "Kd") {
				sStream >> color.r >> color.g >> color.b;
				color.a = 1.f;
			}
		}

		if (fileInput.bad())
			OutFatalError("Error occured while reading file!");

		fileInput.close();
	}

	void Material::WriteMaterialData(size_t frameIndex) {
		MaterialUbo ubo = GetMaterialUbo();
		Buffer* buffer = GetMaterialBuffers()[frameIndex];
		buffer->WriteToIndex(&ubo, materialIndex);
		buffer->FlushIndex(materialIndex);
	}

	void Material::SaveToFile(const char_t* fileLocation) const {
		std::ofstream fileOutput(fileLocation);

		if (!fileOutput)
			OutFatalError("File not found!");

		fileOutput << "way too ballin ngl";

		if(fileOutput.bad())
			OutFatalError("Error occured while reading file!");

		fileOutput.close();
	}

	Material::~Material() {
		materials.erase(materials.begin() + materialIndex, materials.begin() + materialIndex + 1);
		
		for (size_t i = materialIndex; i < materials.size(); i++)
			materials[i]->materialIndex--;
	}
}