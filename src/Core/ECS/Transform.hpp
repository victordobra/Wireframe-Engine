#pragma once

#include "Core/Math/General/VecUtils.hpp"
#include "Core/Math/General/QuatUtils.hpp"

namespace wfe {
	/// @brief A struct representing the position, rotation and size of an entity.
	struct Transform {
		/// @brief The entity's position.
		Vec3f pos = VEC3F_ZERO;
		/// @brief The entity's rotation.
		Quatf rot = QUATF_IDENTITY;
		/// @brief The entity's scale.
		Vec3f scale = VEC3F_ONE;
	};
}