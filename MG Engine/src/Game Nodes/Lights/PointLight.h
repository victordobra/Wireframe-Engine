#pragma once
#include "Node.h"
#include "EngineMath.h"
#include "ColorTypes.h"

namespace mge {
	class PointLight : public Node {
	public:
		Vector3 position;
		Color4 color;
	};
}