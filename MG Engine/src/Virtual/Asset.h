#pragma once
#include "Defines.h"
#include <string>

namespace mge {
	class Asset {
	public:
		Asset() = default;
		Asset(const Asset&) = default;
		Asset(Asset&&) noexcept = default;
		Asset(const char_t* fileLocation);

		virtual void SaveToFile(const char_t* fileLocation) const;

		virtual ~Asset() = default;
	};
}