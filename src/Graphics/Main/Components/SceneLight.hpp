#pragma once

#include "Core/ECS/ComponentType.hpp"
#include "Core/Types/Defines.hpp"
#include "Core/Math/Templates/Vec3.hpp"

namespace wfe {
	/// @brief A component representing a light in the render scene.
	struct SceneLight {
	public:
		/// @brief An enum that represents the types of scene lights.
		enum LightType {
			/// @brief The sun light type, whose direction is constant in the entire scene.
			LIGHT_TYPE_SUN,
			/// @brief The point light type, whose directions diverge from its position.
			LIGHT_TYPE_POINT,
			/// @brief The spot light type, which eluminares in a cone from a point in its direction.
			LIGHT_TYPE_SPOT
		};
		/// @brief A struct containing the info for a sun light.
		struct SunLightInfo {
			/// @brief The light's color.
			Vec3f lightColor;
			/// @brief The light's intensity.
			float lightIntensity;
		};
		/// @brief A struct containing the info for a point light.
		struct PointLightInfo {
			/// @brief The light's color.
			Vec3f lightColor;
			/// @brief The light's intensity.
			float lightIntensity;
			/// @brief The constant term in the inverse quadratic expression for distance scaling.
			float constantScaling;
			/// @brief The linear term in the inverse quadratic expression for distance scaling.
			float linearScaling;
			/// @brief The quadratic term in the inverse quadratic expression for distance scaling.
			float quadraticScaling;
		};
		struct SpotLightInfo {
			/// @brief The light's color.
			Vec3f lightColor;
			/// @brief The light's intensity.
			float lightIntensity;
			/// @brief The angle (in radians) of the cone, pointing in the light's direction, which is fully lit.
			float innerCutoff;
			/// @brief The angle (in radians) of the cone, pointing in the list's direction, which is at least partially lit. Larger than the inner cutoff.
			float outerCutoff;
		};

		/// @brief The light's type.
		LightType lightType;

		union {
			/// @brief The info for a sun light.
			SunLightInfo sunLightInfo;
			/// @brief The info for a point light.
			PointLightInfo pointLightInfo;
			/// @brief The info for a spot light.
			SpotLightInfo spotLightInfo;
		};
	private:
		WFE_COMPONENT_TYPE(SceneLight);
	};
}