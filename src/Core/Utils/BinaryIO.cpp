#include "BinaryIO.hpp"

namespace wfe {
	// Public functions
	uint16_t BinaryReadUint16(std::istream& stream) {
		// Read the value's bytes from the stream
		uint8_t valBuffer[2];
		stream.read((char*)valBuffer, 2);

		// Assemble the value
		return ((uint16_t)valBuffer[0] << 8) | (uint16_t)valBuffer[1];
	}
	uint32_t BinaryReadUint32(std::istream& stream) {
		// Read the value's bytes from the stream
		uint8_t valBuffer[4];
		stream.read((char*)valBuffer, 4);

		// Assemble the value
		return ((uint32_t)valBuffer[0] << 24) | ((uint32_t)valBuffer[1] << 16) | ((uint32_t)valBuffer[2] << 8) | (uint32_t)valBuffer[3];
	}
	uint64_t BinaryReadUint64(std::istream& stream) {
		// Read the value's bytes from the stream
		uint8_t valBuffer[8];
		stream.read((char*)valBuffer, 8);

		// Assemble the value
		return ((uint64_t)valBuffer[0] << 56) | ((uint64_t)valBuffer[1] << 48) | ((uint64_t)valBuffer[2] << 40) | ((uint64_t)valBuffer[3] << 32) | ((uint64_t)valBuffer[4] << 24) | ((uint64_t)valBuffer[5] << 16) | ((uint64_t)valBuffer[6] << 8) | (uint64_t)valBuffer[7];
	}
	float BinaryReadFloat(std::istream& stream) {
		// Read the equivalent IEEE encoding integer and convert it to a float
		uint32_t valInt = BinaryReadUint32(stream);
		return *(float*)&valInt;
	}
	double BinaryReadDouble(std::istream& stream) {
		// Read the equivalent IEEE encoding integer and convert it to a double
		uint64_t valInt = BinaryReadUint64(stream);
		return *(double*)&valInt;
	}

	void BinaryWriteUint16(std::ostream& stream, uint16_t val) {
		// Disassemble the value into its bytes
		uint8_t valBuffer[2];
		valBuffer[0] = (uint8_t)(val >> 8);
		valBuffer[1] = (uint8_t)val;

		// Write the value's bytes to the stream
		stream.write((const char*)valBuffer, 2);
	}
	void BinaryWriteUint32(std::ostream& stream, uint32_t val) {
		// Disassemble the value into its bytes
		uint8_t valBuffer[4];
		valBuffer[0] = (uint8_t)(val >> 24);
		valBuffer[1] = (uint8_t)(val >> 16);
		valBuffer[2] = (uint8_t)(val >> 8);
		valBuffer[3] = (uint8_t)val;

		// Write the value's bytes to the stream
		stream.write((const char*)valBuffer, 4);
	}
	void BinaryWriteUint64(std::ostream& stream, uint64_t val) {
		// Disassemble the value into its bytes
		uint8_t valBuffer[8];
		valBuffer[0] = (uint8_t)(val >> 56);
		valBuffer[1] = (uint8_t)(val >> 48);
		valBuffer[2] = (uint8_t)(val >> 40);
		valBuffer[3] = (uint8_t)(val >> 32);
		valBuffer[4] = (uint8_t)(val >> 24);
		valBuffer[5] = (uint8_t)(val >> 16);
		valBuffer[6] = (uint8_t)(val >> 8);
		valBuffer[7] = (uint8_t)val;

		// Write the value's bytes to the stream
		stream.write((const char*)valBuffer, 8);
	}
	void BinaryWriteFloat(std::ostream& stream, float val) {
		// Write the float's IEEE encoding
		BinaryWriteUint32(stream, *(uint32_t*)&val);
	}
	void BinaryWriteDouble(std::ostream& stream, double val) {
		// Write the double's IEEE encoding
		BinaryWriteUint64(stream, *(uint64_t*)&val);
	}
}