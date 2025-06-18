#pragma once

#include "Core/Types/Defines.hpp"
#include <istream>
#include <ostream>

namespace wfe {
	/// @brief An enum describing all possible parsed image data types. The enum values correspond to the number of bytes in each 
	enum ImageComposition {
		/// @brief The composition of grayscale images. Each pixel contains 1 byte, corresponding to its grayscale component.
		IMAGE_COMPOSITION_GRAYSCALE = 1,
		/// @brief The composition of grayscale images with an alpha component. Each pixel contains 2 bytes, corresponding to its grayscale and alpha components, in that order.
		IMAGE_COMPOSITION_GRAYSCALE_ALPHA = 2,
		/// @brief The composition of color images. Each pixel contains 3 bytes, corresponding to its red, green and blue components, in that order.
		IMAGE_COMPOSITION_RGB = 3,
		/// @brief The composition of color images with an alpha component. Each pixel contains 4 bytes, corresponding to its red, green, blue and alpha components, in that order.
		IMAGE_COMPOSITION_RGBA = 4
	};

	/// @brief Reads an image file, outputting its raw color values.
	/// @param stream The input stream of the file. Must be binary.
	/// @param width A reference to the variable to which the image's width will be written.
	/// @param height A reference to the variable to which the image's height will be written.
	/// @param composition A reference to the variable to which the image data's composition will be written.
	/// @return A pointer to an allocated array of every pixel's data, stored according to the image's composition. Can be later freed with FreeMemory. Set to nullptr if the image's layout is incorrect.
	uint8_t* ReadImageFile(std::istream& stream, uint32_t& width, uint32_t& height, ImageComposition& composition);

	/// @brief Writes a PNG file containing the given raw color values.
	/// @param stream The output stream of the file. Must be binary.
	/// @param width The image's width.
	/// @param height The image's height.
	/// @param composition The image data's composition.
	/// @param data A pointer to an array of every pixel's data, stored according to the image's composition.
	/// @return True if the write was successful, otherwise false.
	bool WritePNGFile(std::ostream& stream, uint32_t width, uint32_t height, ImageComposition composition, uint8_t* data);
	/// @brief Writes a JPEG file containing the given raw color values.
	/// @param stream The output stream of the file. Must be binary.
	/// @param width The image's width.
	/// @param height The image's height.
	/// @param composition The image data's composition.
	/// @param data A pointer to an array of every pixel's data, stored according to the image's composition.
	/// @return True if the write was successful, otherwise false.
	bool WriteJPEGFile(std::ostream& stream, uint32_t width, uint32_t height, ImageComposition composition, uint8_t* data);
	/// @brief Writes a QOI file containing the given raw color values.
	/// @param stream The output stream of the file. Must be binary.
	/// @param width The image's width.
	/// @param height The image's height.
	/// @param composition The image data's composition.
	/// @param data A pointer to an array of every pixel's data, stored according to the image's composition.
	/// @return True if the write was successful, otherwise false.
	bool WriteBMPFile(std::ostream& stream, uint32_t width, uint32_t height, ImageComposition composition, uint8_t* data);
}