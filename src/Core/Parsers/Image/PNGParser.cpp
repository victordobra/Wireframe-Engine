#include "PNGParser.hpp"
#include "Core/Memory/Allocator.hpp"
#include "Core/Utils/BinaryIO.hpp"
#include <cstdlib>
#include <cstring>
#include <vector>

namespace wfe {
	// Macros
#define WFE_ENCODE_CHUNK_TYPE_BE(type) (((uint32_t)type[0] << 24) | ((uint32_t)type[1] << 16) | ((uint32_t)type[2] << 8) | (uint32_t)type[3])

	// Structs
	struct HuffmanTree {
		uint32_t lengthCodeStarts[16];
		uint32_t lengthIndexStarts[16];
		uint32_t symbols[288];
	};
	struct HashTableItem {
		uint32_t buffer;
		uint32_t next;
	};
	struct CompressedInputBuffer {
		uint8_t* data = nullptr;
		size_t dataSize = 0;
		uint32_t cache = 0;

		uint32_t localBitIndex = 0;
		uint32_t byteIndex = 0;
	};
	struct CompressedOutputBuffer {
		std::vector<uint8_t> data;
		uint32_t cache = 0;
		uint32_t localBitIndex = 0;
	};

	// Constants
	static const uint8_t FILE_SIGNATURE[] {
		0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a
	};

	static const uint32_t CRC_TABLE[] {
		0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 
		0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5, 
		0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59, 
		0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 
		0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433, 0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01, 
		0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 
		0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 
		0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f, 0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad, 
		0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1, 
		0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 
		0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b, 0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79, 
		0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d, 
		0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 
		0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777, 0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 
		0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9, 
		0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
	};
	static const uint32_t ADLER32_MOD = 65521;

	static const uint32_t FIXED_ALPHABET_CODE_LENGTHS[] {
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		8, 8, 8, 8, 8, 8, 8, 8
	};
	static const uint32_t FIXED_DISTANCE_CODE_LENGTHS[] {
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
	};
	static const uint32_t CODE_LENGTH_CODE_LENGTHS_ORDER[] {
		16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
	};

	static const uint32_t LZ77_LENGTH_STARTS[] {
		3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258
	};
	static const uint32_t LZ77_LENGTH_BIT_COUNTS[] {
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0
	};
	static const uint32_t LZ77_DISTANCE_STARTS[] {
		1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577
	};
	static const uint32_t LZ77_DISTANCE_BIT_COUNTS[] {
		0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13
	};

	static const uint32_t CHUNK_TYPE_IHDR = WFE_ENCODE_CHUNK_TYPE_BE("IHDR");
	static const uint32_t CHUNK_TYPE_PLTE = WFE_ENCODE_CHUNK_TYPE_BE("PLTE");
	static const uint32_t CHUNK_TYPE_tRNS = WFE_ENCODE_CHUNK_TYPE_BE("tRNS");
	static const uint32_t CHUNK_TYPE_IDAT = WFE_ENCODE_CHUNK_TYPE_BE("IDAT");
	static const uint32_t CHUNK_TYPE_IEND = WFE_ENCODE_CHUNK_TYPE_BE("IEND");

	static const uint32_t SLIDING_WINDOW_SIZE = 32768;
	static const uint32_t HASH_TABLE_BUCKET_COUNT = 32771;
	static const uint32_t MAX_CHAIN_LENGTH = 32;
	static const uint32_t SEARCH_END_LENGTH = 32;

	// Internal helper functions	
	static void RefreshCompressedInputBufferCache(CompressedInputBuffer& buffer) {
		// Remove all read bits from the cache and add upcoming bits in their place
		if(buffer.localBitIndex >= 16) {
			buffer.localBitIndex -= 16;

			buffer.cache >>= 16;
			buffer.cache |= (uint32_t)buffer.data[buffer.byteIndex++] << 8;
			buffer.cache |= (uint32_t)buffer.data[buffer.byteIndex++] << 16;
		} else if(buffer.localBitIndex >= 8) {
			buffer.localBitIndex -= 8;

			buffer.cache >>= 8;
			buffer.cache |= (uint32_t)buffer.data[buffer.byteIndex++] << 16;
		}
	}
	static uint32_t ReadBitValue(uint32_t bitCount, CompressedInputBuffer& buffer) {
		// Compose the value using all bytes in the cache
		uint32_t val = buffer.cache >> buffer.localBitIndex;

		// Exclude all bits not in the value
		val &= (1 << bitCount) - 1;

		// Refresh the compressed buffer's cache
		buffer.localBitIndex += bitCount;
		RefreshCompressedInputBufferCache(buffer);

		return val;
	}
	static uint16_t ReadColorValue(uint8_t bitDepth, const uint8_t* data, uint32_t& bitIndex) {
		// Get the resulting color value
		uint16_t res;

		switch(bitDepth) {
		case 1:
			res = (uint16_t)((data[bitIndex >> 3] >> (bitIndex & 7)) & 1);
			break;
		case 2:
			res = (uint16_t)((data[bitIndex >> 3] >> (bitIndex & 7)) & 3);
			break;
		case 4:
			res = (uint16_t)((data[bitIndex >> 3] >> (bitIndex & 7)) & 15);
			break;
		case 8:
			res = (uint16_t)data[bitIndex >> 3];
			break;
		case 16:
			res = ((uint16_t)data[bitIndex >> 3] << 8) | (uint16_t)data[(bitIndex >> 3) + 1];
			break;
		};

		// Update the bit index and exit the function
		bitIndex += bitDepth;
		return res;
	}

	static void RefreshCompressedOutputBufferCache(CompressedOutputBuffer& buffer) {
		// Remove all written bits from the cache and add upcoming bits in their place
		if(buffer.localBitIndex >= 16) {
			buffer.localBitIndex -= 16;

			buffer.data.push_back((uint8_t)buffer.cache);
			buffer.data.push_back((uint8_t)(buffer.cache >> 8));
			buffer.cache >>= 16;
		} else if(buffer.localBitIndex >= 8) {
			buffer.localBitIndex -= 8;

			buffer.data.push_back((uint8_t)buffer.cache);
			buffer.cache >>= 8;
		}
	}
	static void WriteBitValue(uint32_t bitCount, uint32_t value, CompressedOutputBuffer& buffer) {
		// Append the current value to the cache
		buffer.cache |= value << buffer.localBitIndex;
		buffer.localBitIndex += bitCount;

		// Refresh compressed buffer's cache
		RefreshCompressedOutputBufferCache(buffer);
	}
	static void WriteHuffmanCode(uint32_t code, uint32_t codeLength, CompressedOutputBuffer& buffer) {
		// Reverse the code's bit order
		code = ((code >> 1) & 0x5555) | ((code & 0x5555) << 1);
		code = ((code >> 2) & 0x3333) | ((code & 0x3333) << 2);
		code = ((code >> 4) & 0x0f0f) | ((code & 0x0f0f) << 4);
		code = ((code >> 8) & 0x00ff) | ((code & 0x00ff) << 8);
		code >>= 16 - codeLength;

		// Write the code to the output buffer
		WriteBitValue(codeLength, code, buffer);
	}
	static void WriteFixedHuffmanLiteral(uint8_t byte, CompressedOutputBuffer& buffer) {
		// Write the byte using the fixed Huffman codes
		if(byte < 144) {
			WriteHuffmanCode(0b00110000 + byte, 8, buffer);
		} else {
			WriteHuffmanCode(0b110010000 + (byte - 144), 9, buffer);
		}
	}
	static void WriteFixedHuffmanLengthDistance(uint32_t length, uint32_t distance, CompressedOutputBuffer& buffer) {
		// Get the length symbol
		uint32_t lengthSymbol = (uint32_t)(LZ77_LENGTH_STARTS[16] <= length) << 4;
		lengthSymbol += (uint32_t)(LZ77_LENGTH_STARTS[lengthSymbol + 8] <= length) << 3;
		lengthSymbol += (uint32_t)(LZ77_LENGTH_STARTS[lengthSymbol + 4] <= length) << 2;
		lengthSymbol += (uint32_t)(lengthSymbol < 27 && LZ77_LENGTH_STARTS[lengthSymbol + 2] <= length) << 1;
		lengthSymbol += (uint32_t)(lengthSymbol < 28 && LZ77_LENGTH_STARTS[lengthSymbol + 1] <= length);

		// Write the copy length
		if(lengthSymbol < 23) {
			WriteHuffmanCode(0b0000001 + lengthSymbol, 7, buffer);
		} else {
			WriteHuffmanCode(0b11000000 + (lengthSymbol - 23), 8, buffer);
		}

		WriteBitValue(LZ77_LENGTH_BIT_COUNTS[lengthSymbol], length - LZ77_LENGTH_STARTS[lengthSymbol], buffer);

		// Get the distance symbol
		uint32_t distanceSymbol = (uint32_t)(LZ77_DISTANCE_STARTS[16] <= distance) << 4;
		distanceSymbol += (uint32_t)(LZ77_DISTANCE_STARTS[distanceSymbol + 8] <= distance) << 3;
		distanceSymbol += (uint32_t)(LZ77_DISTANCE_STARTS[distanceSymbol + 4] <= distance) << 2;
		distanceSymbol += (uint32_t)(distanceSymbol < 28 && LZ77_DISTANCE_STARTS[distanceSymbol + 2] <= distance) << 1;
		distanceSymbol += (uint32_t)(distanceSymbol < 29 && LZ77_DISTANCE_STARTS[distanceSymbol + 1] <= distance);

		// Write the copy distance
		WriteHuffmanCode(distanceSymbol, 5, buffer);
		WriteBitValue(LZ77_DISTANCE_BIT_COUNTS[distanceSymbol], distance - LZ77_DISTANCE_STARTS[distanceSymbol], buffer);
	}

	static void BuildHuffmanTree(const uint32_t* lengths, uint32_t symbolCount, HuffmanTree& tree) {
		// Get the number of symbols for every length
		uint32_t lengthCounts[16]{};
		for(uint32_t i = 0; i != symbolCount; ++i)
			++lengthCounts[lengths[i]];
		
		// Get the start indices for every length
		tree.lengthIndexStarts[0] = 0;
		for(uint32_t i = 1; i != 16; ++i)
			tree.lengthIndexStarts[i] = tree.lengthIndexStarts[i - 1] + lengthCounts[i - 1];
		
		// Get the start codes for every length
		tree.lengthCodeStarts[0] = 0;
		tree.lengthCodeStarts[1] = 0;
		for(uint32_t i = 2; i != 16; ++i)
			tree.lengthCodeStarts[i] = tree.lengthCodeStarts[i - 1] + (lengthCounts[i - 1] << (16 - i));
		
		// Add all symbols to the vector, ordered by length
		for(uint32_t i = 0; i != symbolCount; ++i)
			tree.symbols[tree.lengthIndexStarts[lengths[i]]++] = i;

		// Reset the length index starts
		for(uint32_t i = 0; i != 16; ++i)
			tree.lengthIndexStarts[i] -= lengthCounts[i];
	}
	static uint32_t HuffmanTreeGetVal(HuffmanTree& tree, CompressedInputBuffer& buffer) {
		// Read the following 15 bits from the output stream
		uint32_t fullCode = buffer.cache >> buffer.localBitIndex;
		fullCode &= 0x7fff;

		// Remove the read bit order to coincide with Huffmana code order
		fullCode <<= 1;
		fullCode = ((fullCode >> 1) & 0x5555) | ((fullCode & 0x5555) << 1);
		fullCode = ((fullCode >> 2) & 0x3333) | ((fullCode & 0x3333) << 2);
		fullCode = ((fullCode >> 4) & 0x0f0f) | ((fullCode & 0x0f0f) << 4);
		fullCode = ((fullCode >> 8) & 0x00ff) | ((fullCode & 0x00ff) << 8);

		// Binary search for the code's length
		uint32_t length = (uint32_t)(tree.lengthCodeStarts[8] <= fullCode) << 3;
		length += (uint32_t)(tree.lengthCodeStarts[length + 4] <= fullCode) << 2;
		length += (uint32_t)(tree.lengthCodeStarts[length + 2] <= fullCode) << 1;
		length += (uint32_t)(tree.lengthCodeStarts[length + 1] <= fullCode);

		// Update the bit index and refresh the compressed buffer's cache
		buffer.localBitIndex += length;
		RefreshCompressedInputBufferCache(buffer);

		// Get the symbol's index
		uint32_t index = (fullCode - tree.lengthCodeStarts[length]) >> (15 - length);
		index += tree.lengthIndexStarts[length];

		return tree.symbols[index];
	}

	static uint32_t CalculateCRC(uint32_t crc, const uint8_t* data, uint32_t dataLen) {
		// Compute the new CRC using the table
		for(uint32_t i = 0; i != dataLen; ++i)
			crc = CRC_TABLE[(crc & 0xff) ^ data[i]] ^ (crc >> 8);

		return crc;
	}
	static uint32_t CalculateAdler32(const uint8_t* data, uint32_t dataLen) {
		// Compute the two sums
		uint32_t a = 1, b = 0;

		for(uint32_t i = 0; i != dataLen; ++i) {
			a = (a + data[i]) % ADLER32_MOD;
			b = (b + a) % ADLER32_MOD;
		}

		// Combine the two sums
		return (b << 16) + a;
	}
	static int32_t PaethPredictor(int32_t left, int32_t up, int32_t upLeft) {
		// Use a more efficient branchless Paeth predictor
		int32_t threshold = upLeft * 3 - left - up;
		int32_t minVal = (left < up) ? left : up;
		int32_t maxVal = left + up - minVal;
		int32_t t0 = (maxVal <= threshold) ? minVal : upLeft;
		int32_t t1 = (threshold <= minVal) ? maxVal : t0;
		return t1;
	}
	static uint8_t ConvertTo8Bit(uint16_t val, uint8_t bitDepth) {
		// Calculate the result based on the bit depth
		uint8_t res = 0;
		switch(bitDepth) {
		case 1:
			res = (uint8_t)(val | (val << 4));
			res |= res << 2;
			res |= res << 1;
			break;
		case 2:
			res = (uint8_t)(val | (val << 4));
			res |= res << 2;
			break;
		case 4:
			res = (uint8_t)(val | (val << 4));
			break;
		case 8:
			res = (uint8_t)val;
			break;
		case 16:
			res = (uint8_t)(val >> 8);
		}

		return res;
	}

	// Public functions
	bool CheckPNGSignature(std::istream& stream) {
		// Read the first 8 bytes from the file
		std::streampos startPos = stream.tellg();

		uint8_t signature[8];
		if(stream.read((char*)signature, 8).gcount() != 8) {
			stream.seekg(startPos);
			return false;
		}
		stream.seekg(startPos);

		// Check if the signature is correct
		return !memcmp(signature, FILE_SIGNATURE, 8);
	}
	uint8_t* ReadPNGFile(std::istream& stream, uint32_t& width, uint32_t& height) {
		// Get the file's size
		stream.seekg(0, std::ios::end);
		size_t fileSize = stream.tellg();
		stream.seekg(0, std::ios::beg);

		// Check if the signature is correct
		uint8_t signature[8];
		if(stream.read((char*)signature, 8).gcount() != 8)
			return nullptr;
		
		if(memcmp(signature, FILE_SIGNATURE, 8))
			return nullptr;
		
		// Read the file's header
		uint32_t chunkLength = BinaryReadUint32BE(stream);
		uint32_t chunkType = BinaryReadUint32BE(stream);
		
		if(chunkType != CHUNK_TYPE_IHDR || chunkLength != 13) {
			return nullptr;
		}

		// Read the header's data and skip its CRC
		uint8_t headerData[13];
		stream.read((char*)headerData, 13);
		stream.seekg(4, std::ios::cur);

		// Set the file's general info
		width = ((uint32_t)headerData[0] << 24) | ((uint32_t)headerData[1] << 16) | ((uint32_t)headerData[2] << 8) | (uint32_t)headerData[3];
		height = ((uint32_t)headerData[4] << 24) | ((uint32_t)headerData[5] << 16) | ((uint32_t)headerData[6] << 8) | (uint32_t)headerData[7];

		uint8_t bitDepth = headerData[8];
		uint8_t colorType = headerData[9];
		uint8_t compressionMethod = headerData[10];
		uint8_t filterMethod = headerData[11];
		uint8_t interlaceMethod = headerData[12];

		CompressedInputBuffer compressedBuffer;
		compressedBuffer.data = (uint8_t*)AllocMemory(fileSize);

		// Store the pallette pointer, if required
		uint8_t* palette = nullptr;
		uint32_t paletteEntryCount = 0;

		// Store the transparent color, if required
		uint8_t transparentColor[4] { 0x00, 0x00, 0x00, 0x00 };

		// Keep reading chunks until the end of the file
		bool lastChunk = false;
		while(stream && !lastChunk) {
			// Read the current chunk's length and type
			chunkLength = BinaryReadUint32BE(stream);
			chunkType = BinaryReadUint32BE(stream);

			// Check for the chunk's type
			switch(chunkType) {
			case CHUNK_TYPE_IEND: {
				// Check if the chunk is somehow not empty
				if(chunkLength) {
					FreeMemory(palette);
					return nullptr;
				}

				// Exit the main loop
				lastChunk = true;

				break;
			}
			case CHUNK_TYPE_PLTE: {
				// Exit the function if the chunk is not valid
				if(palette || (chunkLength % 3) || colorType == 0 || colorType == 4) {
					FreeMemory(palette);
					return nullptr;
				}

				// Skip the current chunk if it is not required
				if(colorType == 2 || colorType == 6) {
					stream.seekg(chunkLength, std::ios::cur);
					break;
				}

				// Allocate the palette and store its colors
				paletteEntryCount = chunkLength / 3;
				palette = (uint8_t*)AllocMemory(paletteEntryCount * 4);
				if(!palette)
					throw std::bad_alloc();

				for(uint32_t i = 0; i != paletteEntryCount; ++i) {
					stream.read((char*)(palette + i * 4), 3);

					// Set the alpha to the max value by default
					palette[i * 4 + 3] = 0xff;
				}
				
				break;
			}
			case CHUNK_TYPE_tRNS: {
				// Handle the chunk based on the color type
				if(colorType == 3) {
					// Exit the function if the chunk is not valid
					if(!palette || chunkLength != paletteEntryCount) {
						FreeMemory(palette);
						return nullptr;
					}

					// Update the palette's alpha values
					for(uint32_t i = 0; i != paletteEntryCount; ++i)
						stream.read((char*)(palette + i * 4 + 3), 1);
				} else if(colorType == 0) {
					// Exit the function if the chunk is not valid
					if(chunkLength != 2) {
						FreeMemory(palette);
						return nullptr;
					}

					// Convert the grayscale value and update the transparent color
					uint8_t grayscaleFull[2];
					stream.read((char*)grayscaleFull, 2);
					uint8_t grayscale = ConvertTo8Bit(((uint16_t)grayscaleFull[0] << 8) | (uint16_t)grayscaleFull[1], bitDepth);

					transparentColor[0] = grayscale; // R
					transparentColor[1] = grayscale; // G
					transparentColor[2] = grayscale; // B
					transparentColor[3] = 0xff;      // A
				} else if(colorType == 2) {
					// Exit the function if the chunk is not valid
					if(chunkLength != 6) {
						FreeMemory(palette);
						return nullptr;
					}

					// Convert the RGB values and update the transparent color
					uint8_t colorFull[6];
					stream.read((char*)colorFull, 6);

					uint8_t red = ConvertTo8Bit(((uint16_t)colorFull[0] << 8) | (uint16_t)colorFull[1], bitDepth);
					uint8_t green = ConvertTo8Bit(((uint16_t)colorFull[2] << 8) | (uint16_t)colorFull[3], bitDepth);
					uint8_t blue = ConvertTo8Bit(((uint16_t)colorFull[4] << 8) | (uint16_t)colorFull[5], bitDepth);

					transparentColor[0] = red;   // R
					transparentColor[1] = green; // G
					transparentColor[2] = blue;  // B
					transparentColor[3] = 0xff;  // A
				} else {
					// The chunk should not appear; exit the function
					FreeMemory(palette);
					return nullptr;
				}

				break;
			}
			case CHUNK_TYPE_IDAT: {
				// Check if the palette should exist, yet it was not yet declared
				if(colorType == 3 && !palette)
					return nullptr;

				// Add the data of the current chunk to a vector
				stream.read((char*)(compressedBuffer.data + compressedBuffer.dataSize), chunkLength);
				compressedBuffer.dataSize += chunkLength;

				break;
			}
			default: {
				// Skip the chunk's data
				stream.ignore(chunkLength);

				// Exit the function if the current chunk is critical, yet not supported
				if(!(chunkType & 0x20000000)) {
					FreeMemory(palette);
					return nullptr;
				}

				break;
			}
			}

			// Skip the chunk's CRC
			stream.seekg(4, std::ios::cur);
		}

		// Calculate the number of bytes per scanline in the raw image data
		uint32_t bytesPerScanline;
		uint32_t bytesPerPixel;
		switch(colorType) {
		case 0:
		case 3:
			bytesPerScanline = (width * bitDepth + 7) >> 3;
			bytesPerPixel = (bitDepth + 7) >> 3;
			break;
		case 4:
			bytesPerScanline = (width * bitDepth * 2 + 7) >> 3;
			bytesPerPixel = (bitDepth * 2 + 7) >> 3;
			break;
		case 2:
			bytesPerScanline = (width * bitDepth * 3 + 7) >> 3;
			bytesPerPixel = (bitDepth * 3 + 7) >> 3;
			break;
		case 6:
			bytesPerScanline = (width * bitDepth * 4 + 7) >> 3;
			bytesPerPixel = (bitDepth * 4 + 7) >> 3;
			break;
		};

		++bytesPerScanline;

		// Read the compressed chunk's metadata
		uint8_t cmf = compressedBuffer.data[0];
		uint8_t flags = compressedBuffer.data[1];

		if((((uint16_t)cmf * 256 + flags) % 31) || (cmf & 15) != 8 || (flags & 32)) {
			FreeMemory(palette);

			return nullptr;
		}

		// Initiate the compressed buffer
		compressedBuffer.cache = (uint32_t)compressedBuffer.data[2] | ((uint32_t)compressedBuffer.data[3] << 8) | ((uint32_t)compressedBuffer.data[4] << 16);
		compressedBuffer.localBitIndex = 0;
		compressedBuffer.byteIndex = 5;

		// Read every data block and get the raw decompressed bytes
		uint8_t* rawImageData = (uint8_t*)AllocMemory(bytesPerScanline * height);
		size_t rawImageDataTop = 0;

		uint32_t lastBlock = 0;
		uint32_t bitIndex = 16;
		while(!lastBlock) {
			// Read the block's header
			lastBlock = ReadBitValue(1, compressedBuffer);
			uint32_t blockType = ReadBitValue(2, compressedBuffer);

			// Parse the block based on its type
			if(blockType == 0) {
				// Read the data's length
				compressedBuffer.byteIndex -= 3;
				if(compressedBuffer.localBitIndex)
					++compressedBuffer.byteIndex;

				uint16_t dataLength = (uint16_t)compressedBuffer.data[compressedBuffer.byteIndex] | ((uint16_t)compressedBuffer.data[compressedBuffer.byteIndex + 1] << 8);
				uint16_t nDataLength = (uint16_t)compressedBuffer.data[compressedBuffer.byteIndex + 2] | ((uint16_t)compressedBuffer.data[compressedBuffer.byteIndex + 3] << 8);
				compressedBuffer.byteIndex += 4;

				if(dataLength != ~nDataLength) {
					FreeMemory(compressedBuffer.data);
					FreeMemory(palette);
					return nullptr;
				}

				// Add the block's data to the decompressed list
				uint8_t* blockData = compressedBuffer.data + compressedBuffer.byteIndex;
				std::memcpy(rawImageData + rawImageDataTop, blockData, dataLength);
				rawImageDataTop += dataLength;
				compressedBuffer.byteIndex += dataLength;

				// Rebuild the compressed buffer's cache
				compressedBuffer.cache = (uint32_t)compressedBuffer.data[compressedBuffer.byteIndex] | ((uint32_t)compressedBuffer.data[compressedBuffer.byteIndex + 1] << 8) | ((uint32_t)compressedBuffer.data[compressedBuffer.byteIndex + 2] << 16);
				compressedBuffer.localBitIndex = 0;
				compressedBuffer.byteIndex += 3;
			} else if(blockType == 1 || blockType == 2) {
				// Build the Huffman trees
				HuffmanTree alphabetTree;
				HuffmanTree distanceTree;

				if(blockType == 1) {
					// Build the trees using the fixed tree lengths
					BuildHuffmanTree(FIXED_ALPHABET_CODE_LENGTHS, 288, alphabetTree);
					BuildHuffmanTree(FIXED_DISTANCE_CODE_LENGTHS, 32, distanceTree);
				} else {
					// Read the number of literal and distance codes
					uint32_t alphabetCodeCount = ReadBitValue(5, compressedBuffer) + 257;
					uint32_t distanceCodeCount = ReadBitValue(5, compressedBuffer) + 1;

					// Read the number of code length codes
					uint32_t codeLengthCodeCount = ReadBitValue(4, compressedBuffer) + 4;

					// Read all of the code length code lengths
					uint32_t codeLengthCodeLengths[19]{};
					for(uint32_t i = 0; i != codeLengthCodeCount; ++i)
						codeLengthCodeLengths[CODE_LENGTH_CODE_LENGTHS_ORDER[i]] = ReadBitValue(3, compressedBuffer);

					// Build the code length Huffman tree
					HuffmanTree codeLengthTree;
					BuildHuffmanTree(codeLengthCodeLengths, 19, codeLengthTree);

					// Read the lengths for the trees
					uint32_t lengths[320], lengthCount = 0;
					uint32_t totalSymbolCount = alphabetCodeCount + distanceCodeCount;

					while(lengthCount != totalSymbolCount) {
						// Read the current symbol
						uint32_t currentSymbol = HuffmanTreeGetVal(codeLengthTree, compressedBuffer);

						if(currentSymbol == 16) {
							// Read the number of copied lengths
							uint32_t copiedCount = ReadBitValue(2, compressedBuffer) + 3;

							// Copy the last length the given number of times
							uint32_t lastLength = lengths[lengthCount - 1];
							for(uint32_t i = 0; i != copiedCount; ++i)
								lengths[lengthCount++] = lastLength;
						} else if(currentSymbol == 17) {
							// Read the number of null lengths
							uint32_t nullCount = ReadBitValue(3, compressedBuffer) + 3;

							// Insert the given number of null lengths
							for(uint32_t i = 0; i != nullCount; ++i)
								lengths[lengthCount++] = 0;
						} else if(currentSymbol == 18) {
							// Read the number of null lengths
							uint32_t nullCount = ReadBitValue(7, compressedBuffer) + 11;

							// Insert the given number of null lengths
							for(uint32_t i = 0; i != nullCount; ++i)
								lengths[lengthCount++] = 0;
						} else {
							// Insert the current length
							lengths[lengthCount++] = currentSymbol;
						}
					}

					// Build the alphabet and distance Huffman trees
					BuildHuffmanTree(lengths, alphabetCodeCount, alphabetTree);
					BuildHuffmanTree(lengths + alphabetCodeCount, distanceCodeCount, distanceTree);
				}

				// Decompress the given data
				uint32_t currentSymbol = HuffmanTreeGetVal(alphabetTree, compressedBuffer);

				while(currentSymbol != 256) {
					// Parse the current symbol
					if(currentSymbol < 256) {
						// Add the symbol to the raw image data
						rawImageData[rawImageDataTop++] = (uint8_t)currentSymbol;
					} else {
						// Get the length of the copied block
						uint32_t copyLength = ReadBitValue(LZ77_LENGTH_BIT_COUNTS[currentSymbol - 257], compressedBuffer) + LZ77_LENGTH_STARTS[currentSymbol - 257];

						// Get the distance of the copy's block
						uint32_t distanceSymbol = HuffmanTreeGetVal(distanceTree, compressedBuffer);
						uint32_t copyDistance = ReadBitValue(LZ77_DISTANCE_BIT_COUNTS[distanceSymbol], compressedBuffer) + LZ77_DISTANCE_STARTS[distanceSymbol];

						// Copy the given interval from the raw data vector
						if(copyDistance == 1) {
							uint8_t lastByte = rawImageData[rawImageDataTop - 1];
							for(uint32_t i = 0; i != copyLength; ++i)
								rawImageData[rawImageDataTop++] = lastByte;
						} else {
							uint32_t copyStart = (uint32_t)rawImageDataTop - copyDistance;
							for(uint32_t i = 0; i != copyLength; ++i)
								rawImageData[rawImageDataTop++] = rawImageData[copyStart + i];
						}
					}

					// Read the next symbol
					currentSymbol = HuffmanTreeGetVal(alphabetTree, compressedBuffer);
				}
			} else {
				FreeMemory(compressedBuffer.data);
				FreeMemory(palette);
				return nullptr;
			}
		}

		FreeMemory(compressedBuffer.data);

		// Reverse every filter in the raw image data
		for(uint32_t i = 0; i != height; ++i) {
			// Get the filter method for the current scanline
			uint8_t currentFilterMethod = rawImageData[i * bytesPerScanline];

			switch(currentFilterMethod) {
			case 1:
				for(uint32_t j = 1 + bytesPerPixel; j != bytesPerScanline; ++j) {
					// Reverse the sub filter
					rawImageData[i * bytesPerScanline + j] += rawImageData[i * bytesPerScanline + j - bytesPerPixel];
				}

				break;
			case 2:
				// Skip the current scanline if it is the first
				if(!i)
					break;

				for(uint32_t j = 1; j != bytesPerScanline; ++j) {
					// Reverse the up filter
					rawImageData[i * bytesPerScanline + j] += rawImageData[(i - 1) * bytesPerScanline + j];
				}

				break;
			case 3:
				if(i) {
					for(uint32_t j = 1; j != 1 + bytesPerPixel; ++j) {
						// Reverse the average filter
						rawImageData[i * bytesPerScanline + j] += rawImageData[(i - 1) * bytesPerScanline + j] >> 1;
					}
					for(uint32_t j = 1 + bytesPerPixel; j != bytesPerScanline; ++j) {
						// Get the up and left byte values
						uint16_t leftByte = rawImageData[i * bytesPerScanline + j - bytesPerPixel];
						uint16_t upByte = rawImageData[(i - 1) * bytesPerScanline + j];

						// Reverse the average filter
						rawImageData[i * bytesPerScanline + j] += (uint8_t)((leftByte + upByte) >> 1);
					}
				} else {
					for(uint32_t j = 1 + bytesPerPixel; j != bytesPerScanline; ++j) {
						// Reverse the average filter
						rawImageData[i * bytesPerScanline + j] += rawImageData[i * bytesPerScanline + j - bytesPerPixel] >> 1;
					}
				}

				break;
			case 4:
				if(i) {
					for(uint32_t j = 1; j != 1 + bytesPerPixel; ++j) {
						// Reverse the Paeth filter
						rawImageData[i * bytesPerScanline + j] += rawImageData[(i - 1) * bytesPerScanline + j];
					}
					for(uint32_t j = 1 + bytesPerPixel; j != bytesPerScanline; ++j) {
						// Get all neighbouring byte values
						uint8_t leftByte = rawImageData[i * bytesPerScanline + j - bytesPerPixel];
						uint8_t upByte = rawImageData[(i - 1) * bytesPerScanline + j];
						uint8_t upLeftByte = rawImageData[(i - 1) * bytesPerScanline + j - bytesPerPixel];

						// Reverse the Paeth filter
						rawImageData[i * bytesPerScanline + j] += (uint8_t)PaethPredictor(leftByte, upByte, upLeftByte);
					}
				} else {
					for(uint32_t j = 1 + bytesPerPixel; j != bytesPerScanline; ++j) {
						// Reverse the Paeth filter
						rawImageData[i * bytesPerScanline + j] += rawImageData[i * bytesPerScanline + j - bytesPerPixel];
					}
				}

				break;
			}
		}

		// Allocate the image's data
		uint8_t* imageData = (uint8_t*)AllocMemory(width * height * 4);
		if(!imageData)
			throw std::bad_alloc();
		
		// Set every color value in the image data
		switch(colorType) {
		case 0:
			for(uint32_t i = 0; i != height; ++i) {
				// Set the initial bit index and run through the scanline
				uint8_t* scanlineData = rawImageData + i * bytesPerScanline + 1;
				bitIndex = 0;

				for(uint32_t j = 0; j != width; ++j) {
					// Calculate the current pixel's index
					uint32_t pixelIndex = ((height - i - 1) * width + j) << 2;

					// Read the grayscale value
					uint8_t grayscale = ConvertTo8Bit(ReadColorValue(bitDepth, scanlineData, bitIndex), bitDepth);

					imageData[pixelIndex]     = grayscale; // R
					imageData[pixelIndex + 1] = grayscale; // G
					imageData[pixelIndex + 2] = grayscale; // B
					imageData[pixelIndex + 3] = 0xff;      // A
				}
			}

			break;
		case 2:
			if(bitDepth == 8) {
				for(uint32_t i = 0; i != height; ++i) {
					// Run through the scanline
					uint8_t* scanlineData = rawImageData + i * bytesPerScanline + 1;

					for(uint32_t j = 0; j != width; ++j) {
						// Calculate the current pixel's index
						uint32_t pixelIndex = ((height - i - 1) * width + j) << 2;

						imageData[pixelIndex]     = scanlineData[j * 3];     // R
						imageData[pixelIndex + 1] = scanlineData[j * 3 + 1]; // G
						imageData[pixelIndex + 2] = scanlineData[j * 3 + 2]; // B
						imageData[pixelIndex + 3] = 0xff;                    // A
					}
				}
			} else {
				for(uint32_t i = 0; i != height; ++i) {
					// Run through the scanline
					uint8_t* scanlineData = rawImageData + i * bytesPerScanline + 1;

					for(uint32_t j = 0; j != width; ++j) {
						// Calculate the current pixel's index
						uint32_t pixelIndex = ((height - i - 1) * width + j) << 2;

						imageData[pixelIndex]     = scanlineData[j * 6];     // R
						imageData[pixelIndex + 1] = scanlineData[j * 6 + 2]; // G
						imageData[pixelIndex + 2] = scanlineData[j * 6 + 4]; // B
						imageData[pixelIndex + 3] = 0xff;                    // A
					}
				}
			}

			break;
		case 3:
			for(uint32_t i = 0; i != height; ++i) {
				// Set the initial bit index and run through the scanline
				uint8_t* scanlineData = rawImageData + i * bytesPerScanline + 1;
				bitIndex = 0;

				for(uint32_t j = 0; j != width; ++j) {
					// Calculate the current pixel's index
					uint32_t pixelIndex = ((height - i - 1) * width + j) << 2;

					// Read the grayscale value
					uint16_t paletteIndex = ReadColorValue(bitDepth, scanlineData, bitIndex) << 2;

					imageData[pixelIndex]     = palette[paletteIndex];     // R
					imageData[pixelIndex + 1] = palette[paletteIndex + 1]; // G
					imageData[pixelIndex + 2] = palette[paletteIndex + 2]; // B
					imageData[pixelIndex + 3] = palette[paletteIndex + 3]; // A
				}
			}

			break;
		case 4:
			if(bitDepth == 8) {
				for(uint32_t i = 0; i != height; ++i) {
					// Run through the scanline
					uint8_t* scanlineData = rawImageData + i * bytesPerScanline + 1;

					for(uint32_t j = 0; j != width; ++j) {
						// Calculate the current pixel's index
						uint32_t pixelIndex = ((height - i - 1) * width + j) << 2;

						imageData[pixelIndex]     = scanlineData[j * 2];     // R
						imageData[pixelIndex + 1] = scanlineData[j * 2];     // G
						imageData[pixelIndex + 2] = scanlineData[j * 2];     // B
						imageData[pixelIndex + 3] = scanlineData[j * 2 + 1]; // A
					}
				}
			} else {
				for(uint32_t i = 0; i != height; ++i) {
					// Run through the scanline
					uint8_t* scanlineData = rawImageData + i * bytesPerScanline + 1;

					for(uint32_t j = 0; j != width; ++j) {
						// Calculate the current pixel's index
						uint32_t pixelIndex = ((height - i - 1) * width + j) << 2;

						imageData[pixelIndex]     = scanlineData[j * 4];     // R
						imageData[pixelIndex + 1] = scanlineData[j * 4];     // G
						imageData[pixelIndex + 2] = scanlineData[j * 4];     // B
						imageData[pixelIndex + 3] = scanlineData[j * 4 + 2]; // A
					}
				}
			}

			break;
		case 6:
			if(bitDepth == 8) {
				for(uint32_t i = 0; i != height; ++i) {
					// Run through the scanline
					uint8_t* scanlineData = rawImageData + i * bytesPerScanline + 1;

					for(uint32_t j = 0; j != width; ++j) {
						// Calculate the current pixel's index
						uint32_t pixelIndex = ((height - i - 1) * width + j) << 2;

						imageData[pixelIndex]     = scanlineData[j * 4];     // R
						imageData[pixelIndex + 1] = scanlineData[j * 4 + 1]; // G
						imageData[pixelIndex + 2] = scanlineData[j * 4 + 2]; // B
						imageData[pixelIndex + 3] = scanlineData[j * 4 + 3]; // A
					}
				}
			} else {
				for(uint32_t i = 0; i != height; ++i) {
					// Run through the scanline
					uint8_t* scanlineData = rawImageData + i * bytesPerScanline + 1;

					for(uint32_t j = 0; j != width; ++j) {
						// Calculate the current pixel's index
						uint32_t pixelIndex = ((height - i - 1) * width + j) << 2;

						imageData[pixelIndex]     = scanlineData[j * 8];     // R
						imageData[pixelIndex + 1] = scanlineData[j * 8 + 2]; // G
						imageData[pixelIndex + 2] = scanlineData[j * 8 + 4]; // B
						imageData[pixelIndex + 3] = scanlineData[j * 8 + 6]; // A
					}
				}
			}

			break;
		}

		// Free the raw image data and the palette, if it exists
		FreeMemory(rawImageData);
		FreeMemory(palette);

		return imageData;
	}
	bool WritePNGFile(std::ostream& stream, uint32_t width, uint32_t height, const uint8_t* data) {
		// Allocate the raw image data 
		uint32_t rawDataSize = height * (width * 4 + 1);
		uint8_t* rawImageData = (uint8_t*)AllocMemory(rawDataSize + 3);
		if(!rawImageData)
			throw std::bad_alloc();
		
		for(uint32_t i = 0; i != height; ++i) {
			// Estimate the default filter's entropy
			const uint8_t* scanline = data + (height - i - 1) * width * 4;
			const uint8_t* prevScanline = data + (height - i) * width * 4;

			uint32_t bestFilterEntropy = 0;
			uint32_t bestFilter = 0;

			for(uint32_t j = 0; j != width * 4; ++j)
				bestFilterEntropy += (uint32_t)std::abs((int8_t)scanline[j]);
			
			// Estimate the sub filter's entropy
			uint32_t filterEntropy = 0;

			for(uint32_t j = 0; j != 4; ++j)
				filterEntropy += (uint32_t)std::abs((int8_t)scanline[j]);
			for(uint32_t j = 4; j != width * 4; ++j)
				filterEntropy += (uint32_t)std::abs((int8_t)(scanline[j] - scanline[j - 4]));
			
			if(filterEntropy < bestFilterEntropy) {
				bestFilterEntropy = filterEntropy;
				bestFilter = 1;
			}

			if(i) {
				// Estimate the up filter's entropy
				filterEntropy = 0;

				for(uint32_t j = 0; j != width * 4; ++j)
					filterEntropy += (uint32_t)std::abs((int8_t)(scanline[j] - prevScanline[j]));
				
				if(filterEntropy < bestFilterEntropy) {
					bestFilterEntropy = filterEntropy;
					bestFilter = 2;
				}

				// Estimate the average filter's entropy
				filterEntropy = 0;

				for(uint32_t j = 0; j != 4; ++j)
					filterEntropy += (uint32_t)std::abs((int8_t)(scanline[j] - (prevScanline[j] >> 1)));
				for(uint32_t j = 4; j != width * 4; ++j)
					filterEntropy += (uint32_t)std::abs((int8_t)(scanline[j] - (uint8_t)(((uint16_t)scanline[j - 4] + prevScanline[j]) >> 1)));

				if(filterEntropy < bestFilterEntropy) {
					bestFilterEntropy = filterEntropy;
					bestFilter = 3;
				}

				// Estimate the Paeth filter's entropy
				filterEntropy = 0;

				for(uint32_t j = 0; j != 4; ++j)
					filterEntropy += (uint32_t)std::abs((int8_t)(scanline[j] - prevScanline[j]));
				for(uint32_t j = 4; j != width * 4; ++j)
					filterEntropy += (uint32_t)std::abs((int8_t)(scanline[j] - (uint8_t)PaethPredictor(scanline[j - 4], prevScanline[j], prevScanline[j - 4])));

				if(filterEntropy < bestFilterEntropy) {
					bestFilterEntropy = filterEntropy;
					bestFilter = 4;
				}
			}

			// Apply the best filter to the current scanline
			uint8_t* rawScanline = rawImageData + i * (width * 4 + 1) + 1;

			rawScanline[-1] = (uint8_t)bestFilter;

			switch(bestFilter) {
			case 0:
				// Copy the scanline as is
				memcpy(rawScanline, scanline, width * 4);

				break;
			case 1:
				// Apply the sub filter
				for(uint32_t j = 0; j != 4; ++j)
					rawScanline[j] = scanline[j];
				for(uint32_t j = 4; j != width * 4; ++j)
					rawScanline[j] = scanline[j] - scanline[j - 4];

				break;
			case 2:
				// Apply the up filter
				for(uint32_t j = 0; j != width * 4; ++j)
					rawScanline[j] = scanline[j] - prevScanline[j];
				
				break;
			case 3:
				// Apply the average filter
				for(uint32_t j = 0; j != 4; ++j)
					rawScanline[j] = scanline[j] - (prevScanline[j] >> 1);
				for(uint32_t j = 4; j != width * 4; ++j)
					rawScanline[j] = scanline[j] - (uint8_t)(((uint16_t)scanline[j - 4] + prevScanline[j]) >> 1);
				
				break;
			case 4:
				// Apply the Paeth filter
				for(uint32_t j = 0; j != 4; ++j)
					rawScanline[j] = scanline[j] - prevScanline[j];
				for(uint32_t j = 4; j != width * 4; ++j)
					rawScanline[j] = scanline[j] - (uint8_t)PaethPredictor(scanline[j - 4], prevScanline[j], prevScanline[j - 4]);
				
				break;
			}
		}

		// Calculate the checksum for the raw image data
		uint32_t rawChecksum = CalculateAdler32(rawImageData, height * (width * 4 + 1));

		// Allocate the string matching hash table
		HashTableItem* hashTableItems = (HashTableItem*)AllocMemory(sizeof(HashTableItem) * SLIDING_WINDOW_SIZE);
		uint32_t* hashTableBuckets = (uint32_t*)AllocMemory(sizeof(uint32_t) * HASH_TABLE_BUCKET_COUNT);

		for(uint32_t i = 0; i != HASH_TABLE_BUCKET_COUNT; ++i)
			hashTableBuckets[i] = UINT32_T_MAX;
	
		// Write the ZLIB header
		CompressedOutputBuffer compressedBuffer;

		compressedBuffer.data.push_back(0x78); // CMF
		compressedBuffer.data.push_back(0x5e); // FLG

		// Write the block's info
		WriteBitValue(3, 0b011, compressedBuffer);

		// Write the raw image data to the compressed buffer
		if(rawDataSize < 3) {
			for(uint32_t i = 0; i != rawDataSize; ++i) {
				// Write the current byte to the compressed buffer
				WriteFixedHuffmanLiteral(rawImageData[i], compressedBuffer);
			}
		} else {
			// Save a lookahead buffer
			uint32_t lookaheadBuffer = (uint32_t)rawImageData[0] | ((uint32_t)rawImageData[1] << 8) | ((uint32_t)rawImageData[2] << 16);

			for(uint32_t i = 0; i != rawDataSize; ++i) {
				if(i > rawDataSize - 3) {
					// Write the current byte to the compressed buffer
					WriteFixedHuffmanLiteral(rawImageData[i], compressedBuffer);

					continue;
				}

				// Calculate the bucket index for the current sequence
				uint32_t bufferHash = (lookaheadBuffer ^ (lookaheadBuffer >> 16)) * 0x7feb352d;
				bufferHash = (bufferHash ^ (bufferHash >> 15)) * 0x846ca68b;
				bufferHash ^= bufferHash >> 16;

				uint32_t bucketIndex = bufferHash % HASH_TABLE_BUCKET_COUNT;

				// Search for the longest match in the hash table
				uint32_t bestLength = 0, bestDistance = 0;

				for(uint32_t j = 0, index = hashTableBuckets[bucketIndex]; j != MAX_CHAIN_LENGTH && index != UINT32_T_MAX && index + SLIDING_WINDOW_SIZE >= i; ++j, index = hashTableItems[index & (SLIDING_WINDOW_SIZE - 1)].next) {
					// Check if the first three bytes match
					if(hashTableItems[index & (SLIDING_WINDOW_SIZE - 1)].buffer != lookaheadBuffer)
						continue;
					
					// Calculate the match's length
					uint32_t matchLength = 3;
					for(; i + matchLength != rawDataSize && matchLength != 258; ++matchLength) {
						if(rawImageData[index + matchLength] != rawImageData[i + matchLength])
							break;
					}

					// Update the best match, if needed
					if(matchLength > bestLength) {
						bestLength = matchLength;
						bestDistance = i - index;

						if(bestLength >= SEARCH_END_LENGTH)
							break;
					}
				}

				if(bestLength) {
					// Write the length/distance pair to the compressed buffer
					WriteFixedHuffmanLengthDistance(bestLength, bestDistance, compressedBuffer);

					// Add every sequence in the match to the hash table
					for(uint32_t j = 0; j != bestLength; ++j) {
						// Add the current lookahead buffer to the hash table
						hashTableItems[(i + j) & (SLIDING_WINDOW_SIZE - 1)] = {
							.buffer = lookaheadBuffer,
							.next = hashTableBuckets[bucketIndex]
						};
						hashTableBuckets[bucketIndex] = i + j;

						// Update the lookahead buffer and the bucket index
						lookaheadBuffer = (lookaheadBuffer >> 8) | ((uint32_t)rawImageData[i + j + 3] << 16);

						bufferHash = (lookaheadBuffer ^ (lookaheadBuffer >> 16)) * 0x7feb352d;
						bufferHash = (bufferHash ^ (bufferHash >> 15)) * 0x846ca68b;
						bufferHash ^= bufferHash >> 16;

						bucketIndex = bufferHash % HASH_TABLE_BUCKET_COUNT;
					}

					i += bestLength - 1;
				} else {
					// Write the current byte to the compressed buffer
					WriteFixedHuffmanLiteral(rawImageData[i], compressedBuffer);
				
					// Add the current lookahead buffer to the hash table
					hashTableItems[i & (SLIDING_WINDOW_SIZE - 1)] = {
						.buffer = lookaheadBuffer,
						.next = hashTableBuckets[bucketIndex]
					};
					hashTableBuckets[bucketIndex] = i;

					// Update the lookahead buffer
					lookaheadBuffer = (lookaheadBuffer >> 8) | ((uint32_t)rawImageData[i + 3] << 16);
				}
			}
		}

		// Write the end of the block
		WriteHuffmanCode(0b0000000, 7, compressedBuffer);

		// Add all remaining bits to the compressed buffer
		if(compressedBuffer.localBitIndex)
			compressedBuffer.data.push_back((uint8_t)compressedBuffer.cache);
		
		// Write the checksum for the raw image data
		compressedBuffer.data.push_back((uint8_t)rawChecksum);
		compressedBuffer.data.push_back((uint8_t)(rawChecksum >> 8));
		compressedBuffer.data.push_back((uint8_t)(rawChecksum >> 16));
		compressedBuffer.data.push_back((uint8_t)(rawChecksum >> 24));

		// Free the raw image data
		FreeMemory(rawImageData);

		// Write the PNG signature
		stream.write((const char*)FILE_SIGNATURE, 8);

		// Write the IHDR chunk
		BinaryWriteUint32BE(stream, 13);
		stream.write("IHDR", 4);

		uint8_t headerData[] {
			(uint8_t)(width >> 24), (uint8_t)(width >> 16), (uint8_t)(width >> 8), (uint8_t)width,
			(uint8_t)(height >> 24), (uint8_t)(height >> 16), (uint8_t)(height >> 8), (uint8_t)height,
			8, 6, 0, 0, 0
		};

		stream.write((const char*)headerData, 13);

		// Write the IHDR chunk's CRC
		uint32_t crc = CalculateCRC(0xffffffff, (const uint8_t*)"IHDR", 4);
		crc = CalculateCRC(crc, (const uint8_t*)headerData, 13);	
		crc ^= 0xffffffff;

		BinaryWriteUint32BE(stream, crc);

		// Write multiple IDAT chunks containing the compressed data
		for(uint32_t i = 0; i < compressedBuffer.data.size(); i += 65536) {
			// Calculate the current chunk's size
			uint32_t chunkSize = (uint32_t)compressedBuffer.data.size() - i;
			if(chunkSize > 65536)
				chunkSize = 65536;
			
			// Write the chunk's length and type
			BinaryWriteUint32BE(stream, chunkSize);
			stream.write("IDAT", 4);

			// Write the chunk's data
			stream.write((const char*)(compressedBuffer.data.data() + i), chunkSize);

			// Write the chunk's CRC
			crc = CalculateCRC(0xffffffff, (const uint8_t*)"IDAT", 4);
			crc = CalculateCRC(crc, compressedBuffer.data.data() + i, chunkSize);
			crc ^= 0xffffffff;

			BinaryWriteUint32BE(stream, crc);
		}

		// Write the IEND chunk
		BinaryWriteUint32BE(stream, 0);
		stream.write("IEND", 4);
		BinaryWriteUint32BE(stream, CalculateCRC(0xffffffff, (const uint8_t*)"IEND", 4) ^ 0xffffffff);

		return true;
	}
}