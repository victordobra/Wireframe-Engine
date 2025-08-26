#pragma once

#include "PNGParser.hpp"
#include "TGAParser.hpp"
#include "Core/Types/Defines.hpp"
#include <istream>
#include <ostream>

namespace wfe {
	/// @brief Reads an image file, outputting its raw color values.
	/// @param stream The input stream of the file. Must be binary.
	/// @param width A reference to the variable to which the image's width will be written.
	/// @param height A reference to the variable to which the image's height will be written.
	/// @return A pointer to an allocated array of every pixel's data, stored as sequences of reg, green, blue and alpha values for every pixel. Can be later freed with FreeMemory. Set to nullptr if the image's layout is incorrect.
	uint8_t* ReadImageFile(std::istream& stream, uint32_t& width, uint32_t& height);
}