#pragma once

#include "Core/Math/Vector3.hpp"
#include "Core/Math/Quaternion.hpp"

namespace wfe {
	/// @brief A struct representing the position, rotation and size of an entity.
	struct Transform {
		/// @brief The entity's position.
		Vector3 pos = Vector3::ZERO;
		/// @brief The entity's rotation.
		Quaternion rot = Quaternion::IDENTITY;
		/// @brief The entity's scale.
		Vector3 scale = Vector3::ONE;
	};
}