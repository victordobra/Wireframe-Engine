#pragma once

#include "Core/ECS/ComponentType.hpp"
#include "Core/Types/Defines.hpp"
#include "Core/Math/Vector3.hpp"

namespace wfe {
	/// @brief A component representing a light in the render scene.
	struct SceneLight {
	public:
		/// @brief An enum that represents the types of scene lights.
		enum LightType {
			/// @brief The ambient light type, which illuminates all objects.
			LIGHT_TYPE_AMBIENT,
			/// @brief The sun light type, whose direction is constant in the entire scene.
			LIGHT_TYPE_SUN,
			/// @brief The point light type, whose directions diverge from its position.
			LIGHT_TYPE_POINT
		};

		/// @brief The light's type.
		LightType lightType;
		/// @brief The light's color.
		Vector3 lightColor;
		/// @brief The light's intensity.
		float lightIntensity;
	private:
		WFE_COMPONENT_TYPE(SceneLight);
	};
}