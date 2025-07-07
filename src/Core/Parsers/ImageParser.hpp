#pragma once

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

	/// @brief Writes a PNG file containing the given raw color values.
	/// @param stream The output stream of the file. Must be binary.
	/// @param width The image's width.
	/// @param height The image's height.
	/// @param data A pointer to an array of every pixel's data, stored as sequences of reg, green, blue and alpha values for every pixel.
	/// @return True if the write was successful, otherwise false.
	bool WritePNGFile(std::ostream& stream, uint32_t width, uint32_t height, uint8_t* data);
	/// @brief Writes a JPEG file containing the given raw color values.
	/// @param stream The output stream of the file. Must be binary.
	/// @param width The image's width.
	/// @param height The image's height.
	/// @param data A pointer to an array of every pixel's data, stored as sequences of reg, green, blue and alpha values for every pixel.
	/// @return True if the write was successful, otherwise false.
	bool WriteJPEGFile(std::ostream& stream, uint32_t width, uint32_t height, uint8_t* data);
	/// @brief Writes a QOI file containing the given raw color values.
	/// @param stream The output stream of the file. Must be binary.
	/// @param width The image's width.
	/// @param height The image's height.
	/// @param data A pointer to an array of every pixel's data, stored as sequences of reg, green, blue and alpha values for every pixel.
	/// @return True if the write was successful, otherwise false.
	bool WriteBMPFile(std::ostream& stream, uint32_t width, uint32_t height, uint8_t* data);
}