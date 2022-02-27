#pragma once
#include "Asset.h"
#include "ColorTypes.h"
#include "ShaderData.h"
#include <vector>

namespace mge {
	class Material : public Asset {
	public:
		static std::vector<Material*> materials;

		size_t materialIndex{0};
		Color4 color{0.f, 0.f, 0.f, 0.f};

		Material();
		Material(const Material& other);
		Material(Material&& other) noexcept;
		Material(const char_t* fileLocation);

		Material& operator=(const Material& other) { color = other.color; return *this; }
		Material& operator=(Material&& other) noexcept { color = other.color; return *this; }

		MaterialUbo GetMaterialUbo() { return { color }; }
		void WriteMaterialData(size_t imageIndex);

		void SaveToFile(const char_t* fileLocation) const override;

		~Material();
	};
}