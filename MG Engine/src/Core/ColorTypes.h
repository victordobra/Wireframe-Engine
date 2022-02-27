#pragma once
#include "Defines.h"

namespace mge {
	struct Color3 {
		union {
			float32_t elements[3]{ 0.f, 0.f, 0.f };

			struct {
				float32_t r, g, b;
			};
		};
	};

	struct Color4 {
		union {
			float32_t elements[4]{ 0.f, 0.f, 0.f, 0.f };

			struct {
				float32_t r, g, b, a;
			};
		};
	};
}