#include "TGAParser.hpp"
#include "Core/Memory/Allocator.hpp"
#include "Core/Utils/BinaryIO.hpp"
#include <string.h>

namespace wfe {
	// Constants
	static const char TGA_END_SIGNATURE[] = "TRUEVISION-XFILE.";

	// Public functions
	bool CheckTGASignature(std::istream& stream) {
		// The TGA file has no signature; simply check if the color map and image type are valid
		uint8_t header[3];
		if(stream.read((char*)header, 3).gcount() != 3) {
			stream.seekg(0, std::ios::beg);
			return false;
		}
		stream.seekg(0, std::ios::beg);

		if(header[1] != 0 && header[1] != 1)
			return false;
		if(header[2] != 1 && header[2] != 2 && header[2] != 3 && header[2] != 9 && header[2] != 10 && header[2] != 11)
			return false;
		
		return true;
	}
	uint8_t* ReadTGAFile(std::istream& stream, uint32_t& width, uint32_t& height) {
		// Read the TGA file's header
		uint8_t header[18];
		stream.read((char*)header, 18);

		uint8_t idLength = header[0];
		uint8_t colorMapType = header[1];
		uint8_t imageType = header[2];

		uint16_t firstColorMapEntryIndex = (uint16_t)header[3] | ((uint16_t)header[4] << 8);
		uint16_t colorMapLength = (uint16_t)header[5] | ((uint16_t)header[6] << 8);
		uint8_t colorMapEntrySize = header[7];

		uint16_t xOrigin = (uint16_t)header[8] | ((uint16_t)header[9] << 8);
		uint16_t yOrigin = (uint16_t)header[10] | ((uint16_t)header[11] << 8);
		width = (uint32_t)header[12] | ((uint32_t)header[13] << 8);
		height = (uint32_t)header[14] | ((uint32_t)header[15] << 8);
		uint8_t pixelDepth = header[16];
		uint8_t imageDescriptor = header[17];

		// Reformat the image type
		uint8_t runLengthEncoded = (imageType >> 3) & 1;
		imageType &= 7;

		// Skip the image ID field
		stream.seekg(idLength, std::ios::cur);

		// Check if image has a color map
		uint8_t* colorMap = nullptr;

		if(colorMapType) {
			// Skip to the first entry index
			stream.seekg(firstColorMapEntryIndex, std::ios::cur);

			// Allocate memory for the color map
			colorMap = (uint8_t*)AllocMemory(colorMapLength * 4);
			if(!colorMap)
				throw std::bad_alloc();
			
			// Read the color map and convert to 32-bit RGBA
			switch(colorMapEntrySize) {
			case 15:
			case 16:
				for(uint32_t i = 0; i != colorMapLength; ++i) {
					// Read the 16-bit color
					uint16_t color = BinaryReadUint16LE(stream);

					// Get and convert the red, green and blue components
					uint16_t rawRed = (color >> 10) & 31;
					uint16_t rawGreen = (color >> 5) & 31;
					uint16_t rawBlue = color & 31;

					colorMap[i * 4] = (uint8_t)((rawRed << 3) | (rawRed >> 2));
					colorMap[i * 4 + 1] = (uint8_t)((rawGreen << 3) | (rawGreen >> 2));
					colorMap[i * 4 + 2] = (uint8_t)((rawBlue << 3) | (rawBlue >> 2));

					// Set the palette entry to fully opaque
					// (according to stb comments, 16-bit colors don't always have the alpha bit set correctly)
					colorMap[i * 4 + 3] = 0xff;
				}

				break;
			case 24:
				for(uint32_t i = 0; i != colorMapLength; ++i) {
					// Read the RGB values and set alpha to fully opaque
					stream.read((char*)(colorMap + i * 4 + 2), 1); // B
					stream.read((char*)(colorMap + i * 4 + 1), 1); // G
					stream.read((char*)(colorMap + i * 4), 1);     // R
					colorMap[i * 4 + 3] = 0xff;                    // A
				}

				break;
			case 32:
				for(uint32_t i = 0; i != colorMapLength; ++i) {
					// Read the RGBA values
					stream.read((char*)(colorMap + i * 4 + 2), 1); // B
					stream.read((char*)(colorMap + i * 4 + 1), 1); // G
					stream.read((char*)(colorMap + i * 4), 1);     // R
					stream.read((char*)(colorMap + i * 4 + 3), 1); // A
				}

				break;
			default:
				FreeMemory(colorMap);
				return nullptr;
			}
		}

		// Allocate memory for the image data
		uint8_t* imageData = (uint8_t*)AllocMemory(width * height * 4);
		if(!imageData)
			throw std::bad_alloc();
		
		// Read the image data
		if(runLengthEncoded) {
			switch(imageType) {
			case 1:
				switch(pixelDepth) {
				case 8:
					for(uint32_t i = 0; i != width * height;) {
						// Read the packet header
						uint8_t packetHeader;
						stream.read((char*)&packetHeader, 1);

						uint8_t runLength = packetHeader >> 7;
						uint32_t count = (uint32_t)(packetHeader & 0x7f) + 1;

						if(runLength) {
							// Read the color index
							uint8_t colorIndex;
							stream.read((char*)&colorIndex, 1);

							for(uint32_t j = 0; j != count; ++j, ++i) {
								// Get the color from the color map
								imageData[i * 4] = colorMap[(uint32_t)colorIndex * 4];         // R
								imageData[i * 4 + 1] = colorMap[(uint32_t)colorIndex * 4 + 1]; // G
								imageData[i * 4 + 2] = colorMap[(uint32_t)colorIndex * 4 + 2]; // B
								imageData[i * 4 + 3] = colorMap[(uint32_t)colorIndex * 4 + 3]; // A
							}
						} else {
							for(uint32_t j = 0; j != count; ++j, ++i) {
								// Read the color index
								uint8_t colorIndex;
								stream.read((char*)&colorIndex, 1);

								// Get the color from the color map
								imageData[i * 4] = colorMap[(uint32_t)colorIndex * 4];         // R
								imageData[i * 4 + 1] = colorMap[(uint32_t)colorIndex * 4 + 1]; // G
								imageData[i * 4 + 2] = colorMap[(uint32_t)colorIndex * 4 + 2]; // B
								imageData[i * 4 + 3] = colorMap[(uint32_t)colorIndex * 4 + 3]; // A
							}

						}
					}

					break;
				case 15:
				case 16:
					for(uint32_t i = 0; i != width * height;) {
						// Read the packet header
						uint8_t packetHeader;
						stream.read((char*)&packetHeader, 1);

						uint8_t runLength = packetHeader >> 7;
						uint32_t count = (uint32_t)(packetHeader & 0x7f) + 1;

						if(runLength) {
							// Read the color index
							uint32_t colorIndex = (uint32_t)BinaryReadUint16LE(stream) & ((1 << pixelDepth) - 1);

							for(uint32_t j = 0; j != count; ++j, ++i) {
								// Get the color from the color map
								imageData[i * 4] = colorMap[(uint32_t)colorIndex * 4];         // R
								imageData[i * 4 + 1] = colorMap[(uint32_t)colorIndex * 4 + 1]; // G
								imageData[i * 4 + 2] = colorMap[(uint32_t)colorIndex * 4 + 2]; // B
								imageData[i * 4 + 3] = colorMap[(uint32_t)colorIndex * 4 + 3]; // A
							}
						} else {
							for(uint32_t j = 0; j != count; ++j, ++i) {
								// Read the color index
								uint32_t colorIndex = (uint32_t)BinaryReadUint16LE(stream) & ((1 << pixelDepth) - 1);

								// Get the color from the color map
								imageData[i * 4] = colorMap[(uint32_t)colorIndex * 4];         // R
								imageData[i * 4 + 1] = colorMap[(uint32_t)colorIndex * 4 + 1]; // G
								imageData[i * 4 + 2] = colorMap[(uint32_t)colorIndex * 4 + 2]; // B
								imageData[i * 4 + 3] = colorMap[(uint32_t)colorIndex * 4 + 3]; // A
							}

						}
					}

					break;
				default:
					FreeMemory(imageData);
					FreeMemory(colorMap);
					return nullptr;
				}

				break;
			case 2:
				switch(pixelDepth) {
				case 15:
				case 16:
					for(uint32_t i = 0; i != width * height;) {
						// Read the packet header
						uint8_t packetHeader;
						stream.read((char*)&packetHeader, 1);

						uint8_t runLength = packetHeader >> 7;
						uint32_t count = (uint32_t)(packetHeader & 0x7f) + 1;

						if(runLength) {
							// Read the 16-bit color
							uint16_t color = BinaryReadUint16LE(stream);

							// Get and convert the red, green and blue components
							uint16_t rawRed = (color >> 10) & 31;
							uint16_t rawGreen = (color >> 5) & 31;
							uint16_t rawBlue = color & 31;

							rawRed = (rawRed << 3) | (rawRed >> 2);
							rawGreen = (rawGreen << 3) | (rawGreen >> 2);
							rawBlue = (rawBlue << 3) | (rawBlue >> 2);

							for(uint32_t j = 0; j != count; ++j, ++i) {
								// Set the pixel color
								imageData[i * 4] = (uint8_t)rawRed;       // R
								imageData[i * 4 + 1] = (uint8_t)rawGreen; // G
								imageData[i * 4 + 2] = (uint8_t)rawBlue;  // B
								imageData[i * 4 + 3] = 0xff;              // A
							}
						} else {
							for(uint32_t j = 0; j != count; ++j, ++i) {
								// Read the 16-bit color
								uint16_t color = BinaryReadUint16LE(stream);

								// Get and convert the red, green and blue components
								uint16_t rawRed = (color >> 10) & 31;
								uint16_t rawGreen = (color >> 5) & 31;
								uint16_t rawBlue = color & 31;
							
								imageData[i * 4] = (uint8_t)((rawRed << 3) | (rawRed >> 2));
								imageData[i * 4 + 1] = (uint8_t)((rawGreen << 3) | (rawGreen >> 2));
								imageData[i * 4 + 2] = (uint8_t)((rawBlue << 3) | (rawBlue >> 2));

								// Set the pixel to fully opaque
								// (according to stb comments, 16-bit colors don't always have the alpha bit set correctly)
								imageData[i * 4 + 3] = 0xff;
							}

						}
					}

					break;
				case 24:
					for(uint32_t i = 0; i != width * height;) {
						// Read the packet header
						uint8_t packetHeader;
						stream.read((char*)&packetHeader, 1);

						uint8_t runLength = packetHeader >> 7;
						uint32_t count = (uint32_t)(packetHeader & 0x7f) + 1;

						if(runLength) {
							// Read the 24-bit color
							uint16_t color[4];
							stream.read((char*)(color + 2), 1); // B
							stream.read((char*)(color + 1), 1); // G
							stream.read((char*)color, 1);       // R
							color[3] = 0xff;                    // A

							for(uint32_t j = 0; j != count; ++j, ++i) {
								// Set the pixel color
								memcpy(imageData + i * 4, color, 4);
							}
						} else {
							for(uint32_t j = 0; j != count; ++j, ++i) {
								// Read the RGB values and set alpha to fully opaque
								stream.read((char*)(imageData + i * 4 + 2), 1); // B
								stream.read((char*)(imageData + i * 4 + 1), 1); // G
								stream.read((char*)(imageData + i * 4), 1);     // R
								imageData[i * 4 + 3] = 0xff;                    // A
							}

						}
					}

					break;
				case 32:
					for(uint32_t i = 0; i != width * height;) {
						// Read the packet header
						uint8_t packetHeader;
						stream.read((char*)&packetHeader, 1);

						uint8_t runLength = packetHeader >> 7;
						uint32_t count = (uint32_t)(packetHeader & 0x7f) + 1;

						if(runLength) {
							// Read the 32-bit color
							uint16_t color[4];
							stream.read((char*)(color + 2), 1); // B
							stream.read((char*)(color + 1), 1); // G
							stream.read((char*)color, 1);       // R
							stream.read((char*)(color + 3), 1); // A

							for(uint32_t j = 0; j != count; ++j, ++i) {
								// Set the pixel color
								memcpy(imageData + i * 4, color, 4);
							}
						} else {
							for(uint32_t j = 0; j != count; ++j, ++i) {
								// Read the RGBA values
								stream.read((char*)(imageData + i * 4 + 2), 1); // B
								stream.read((char*)(imageData + i * 4 + 1), 1); // G
								stream.read((char*)(imageData + i * 4), 1);     // R
								stream.read((char*)(imageData + i * 4 + 3), 1); // A
							}
						}
					}

					break;
				default:
					FreeMemory(imageData);
					FreeMemory(colorMap);
					return nullptr;
				}

				break;
			case 3:
				// Check if the pixel depth is valid
				if(pixelDepth != 8)
					return nullptr;

				for(uint32_t i = 0; i != width * height; ++i) {
					// Read the grayscale value
					uint8_t gray;
					stream.read((char*)&gray, 1);

					// Set the red, green, blue and alpha components
					imageData[i * 4] = gray;     // R
					imageData[i * 4 + 1] = gray; // G
					imageData[i * 4 + 2] = gray; // B
					imageData[i * 4 + 3] = 0xff; // A
				}
			}
		} else {
			switch(imageType) {
			case 1:
				switch(pixelDepth) {
				case 8:
					for(uint32_t i = 0; i != width * height; ++i) {
						// Read the color index
						uint8_t colorIndex;
						stream.read((char*)&colorIndex, 1);

						// Get the color from the color map
						imageData[i * 4] = colorMap[(uint32_t)colorIndex * 4];         // R
						imageData[i * 4 + 1] = colorMap[(uint32_t)colorIndex * 4 + 1]; // G
						imageData[i * 4 + 2] = colorMap[(uint32_t)colorIndex * 4 + 2]; // B
						imageData[i * 4 + 3] = colorMap[(uint32_t)colorIndex * 4 + 3]; // A
					}

					break;
				case 15:
				case 16:
					for(uint32_t i = 0; i != width * height; ++i) {
						// Read the color index
						uint32_t colorIndex = (uint32_t)BinaryReadUint16LE(stream) & ((1 << pixelDepth) - 1);

						// Get the color from the color map
						imageData[i * 4] = colorMap[colorIndex * 4];         // R
						imageData[i * 4 + 1] = colorMap[colorIndex * 4 + 1]; // G
						imageData[i * 4 + 2] = colorMap[colorIndex * 4 + 2]; // B
						imageData[i * 4 + 3] = colorMap[colorIndex * 4 + 3]; // A
					}

					break;
				default:
					FreeMemory(imageData);
					FreeMemory(colorMap);
					return nullptr;
				}

				break;
			case 2:
				switch(pixelDepth) {
				case 15:
				case 16:
					for(uint32_t i = 0; i != width * height; ++i) {
						// Read the 16-bit color
						uint16_t color = BinaryReadUint16LE(stream);

						// Get and convert the red, green and blue components
						uint16_t rawRed = (color >> 10) & 31;
						uint16_t rawGreen = (color >> 5) & 31;
						uint16_t rawBlue = color & 31;

						imageData[i * 4] = (uint8_t)((rawRed << 3) | (rawRed >> 2));
						imageData[i * 4 + 1] = (uint8_t)((rawGreen << 3) | (rawGreen >> 2));
						imageData[i * 4 + 2] = (uint8_t)((rawBlue << 3) | (rawBlue >> 2));

						// Set the pixel to fully opaque
						// (according to stb comments, 16-bit colors don't always have the alpha bit set correctly)
						imageData[i * 4 + 3] = 0xff;
					}

					break;
				case 24:
					for(uint32_t i = 0; i != width * height; ++i) {
						// Read the RGB values and set alpha to fully opaque
						stream.read((char*)(imageData + i * 4 + 2), 1); // B
						stream.read((char*)(imageData + i * 4 + 1), 1); // G
						stream.read((char*)(imageData + i * 4), 1);     // R
						imageData[i * 4 + 3] = 0xff;                    // A
					}

					break;
				case 32:
					for(uint32_t i = 0; i != width * height; ++i) {
						// Read the RGBA values
						stream.read((char*)(imageData + i * 4 + 2), 1); // B
						stream.read((char*)(imageData + i * 4 + 1), 1); // G
						stream.read((char*)(imageData + i * 4), 1);     // R
						stream.read((char*)(imageData + i * 4 + 3), 1); // A
					}

					break;
				default:
					FreeMemory(imageData);
					FreeMemory(colorMap);
					return nullptr;
				}

				break;
			case 3:
				// Check if the pixel depth is valid
				if(pixelDepth != 8)
					return nullptr;

				for(uint32_t i = 0; i != width * height; ++i) {
					// Read the grayscale value
					uint8_t gray;
					stream.read((char*)&gray, 1);

					// Set the red, green, blue and alpha components
					imageData[i * 4] = gray;     // R
					imageData[i * 4 + 1] = gray; // G
					imageData[i * 4 + 2] = gray; // B
					imageData[i * 4 + 3] = 0xff; // A
				}
			}
		}

		// Flip the image horizontally, if required
		if(imageDescriptor & 16) {
			for(uint32_t i = 0; i != height; ++i) {
				for(uint32_t j = 0; j != width >> 1; ++j) {
					// Get the old and new coordinates
					uint32_t oldIndex = (i * width + j) * 4;
					uint32_t newIndex = (i * width + width - j - 1) * 4;

					// Swap the pixels
					uint8_t temp[4];
					memcpy(temp, imageData + oldIndex, 4);
					memcpy(imageData + oldIndex, imageData + newIndex, 4);
					memcpy(imageData + newIndex, temp, 4);
				}
			}
		}

		// Flip the image vertically, if required
		if(imageDescriptor & 32) {
			for(uint32_t i = 0; i != height >> 1; ++i) {
				for(uint32_t j = 0; j != width; ++j) {
					// Get the old and new coordinates
					uint32_t oldIndex = (i * width + j) * 4;
					uint32_t newIndex = ((height - i - 1) * width + j) * 4;

					// Swap the pixels
					uint8_t temp[4];
					memcpy(temp, imageData + oldIndex, 4);
					memcpy(imageData + oldIndex, imageData + newIndex, 4);
					memcpy(imageData + newIndex, temp, 4);
				}
			}
		}

		// Free the color map
		FreeMemory(colorMap);

		return imageData;
	}
	bool WriteTGAFile(std::ostream& stream, uint32_t width, uint32_t height, const uint8_t* data) {
		// Write the TGA file's header
		uint8_t header[] {
			0, // No ID
			0, // No color map
			2, // Uncompressed true-color image
			0, 0, 0, 0, 0, // Color map info
			0, 0, // X origin
			0, 0, // Y origin
			(uint8_t)width,  (uint8_t)(width >> 8),  // Width
			(uint8_t)height, (uint8_t)(height >> 8), // Height
			32, // 32 bits per pixel
			8   // 8 alpha bite, default order
		};
		stream.write((const char*)header, 18);

		// Write the image data
		for(uint32_t i = 0; i != width * height; ++i) {
			// Write the RGBA values
			stream.write((const char*)(data + i * 4 + 2), 1); // B
			stream.write((const char*)(data + i * 4 + 1), 1); // G
			stream.write((const char*)(data + i * 4), 1);     // R
			stream.write((const char*)(data + i * 4 + 3), 1); // A
		}

		// Write the file footer
		uint8_t footerInfo[8] {
			0, 0, 0, 0, // No extension area
			0, 0, 0, 0  // No developer directory
		};

		stream.write((const char*)footerInfo, 8);
		stream.write(TGA_END_SIGNATURE, 18);

		return true;
	}
}