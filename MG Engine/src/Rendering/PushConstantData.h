#pragma once
#include "Matrix4x4.h"

namespace mge {
	struct PushConstantData {
		Matrix4x4 meshRot;
		Matrix4x4 mesh;
	};
}