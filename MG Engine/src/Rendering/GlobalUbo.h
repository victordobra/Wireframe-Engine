#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"

namespace mge {
	struct GlobalUbo {
		Matrix4x4 view;
		Matrix4x4 projection;
		
		Vector4 ambientLightColor{1.0f, 1.0f, 1.0f, 0.02f};
		Vector3 lightPosition;
		alignas(16) Vector4 lightColor{1.0f, 1.0f, 1.0f, 1.0f};
	};
}