#pragma once
#include "Asset.h"
#include "ColorTypes.h"
#include "ShaderData.h"
#include "Image.h"
#include <vector>

namespace mge {
	class Material : public Asset {
	public:
		static std::vector<Material*> materials;
		size_t materialIndex{0};

		Color4 color{0.f, 0.f, 0.f, 0.f};
		Image* image{nullptr};

		Material();
		Material(const Material&) = delete;
		Material(Material&&) noexcept = delete;
		Material(const char_t* fileLocation);

		Material& operator=(const Material&) = delete;
		Material& operator=(Material&&) noexcept = delete;

		MaterialUbo GetMaterialUbo() { return { color, (uint32_t)materialIndex }; }
		void WriteMaterialData(size_t imageIndex);

		void SaveToFile(const char_t* fileLocation) const override;

		~Material();
	};
}