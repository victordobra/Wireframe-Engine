#pragma once

#include "Core/Types/Defines.hpp"

namespace wfe {
	/// @brief A general template for a mathematical vector of size N, using the numerical type T.
	/// @tparam N The size of the vector.
	/// @tparam T The numerical type of the vector's components.
	template<size_t N, class T>
	class Vec;
}