#pragma once
#include "Node.h"
#include "EngineMath.h"
#include "ColorTypes.h"

namespace mge {
	class DirectionalLight : public Node {
	public:
		Vector3 direction;
		Color4 color;
	};
}