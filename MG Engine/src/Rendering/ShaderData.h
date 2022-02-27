#pragma once
#include "EngineMath.h"
#include "ColorTypes.h"

namespace mge {
	constexpr uint32_t MAX_LIGHT_COUNT = 20;

	struct CameraUbo {
		Matrix4x4 view;
		Matrix4x4 projection;
	};
	struct LightingUbo {
		Vector4 ambientLightColor{ 1.f, 1.f, 1.f, 0.1f };

		Vector4 directionalLightDirections[MAX_LIGHT_COUNT];
		Color4 directionalLightColors[MAX_LIGHT_COUNT];

		Vector4 pointLightPositions[MAX_LIGHT_COUNT];
		Color4 pointLightColors[MAX_LIGHT_COUNT];

		uint32_t directionalLightCount{0}, pointLightCount{0};
	};
	struct MaterialUbo {
		Color4 objectColor;
	};

	struct PushConstantData {
		Matrix4x4 meshRot;
		Matrix4x4 mesh;
	};
}