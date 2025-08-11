#include "BinaryIO.hpp"

namespace wfe {
	// Public functions
	uint16_t BinaryReadUint16BE(std::istream& stream) {
		// Read the value's bytes from the stream
		uint8_t valBuffer[2];
		stream.read((char*)valBuffer, 2);

		// Assemble the value
		return ((uint16_t)valBuffer[0] << 8) | (uint16_t)valBuffer[1];
	}
	uint32_t BinaryReadUint32BE(std::istream& stream) {
		// Read the value's bytes from the stream
		uint8_t valBuffer[4];
		stream.read((char*)valBuffer, 4);

		// Assemble the value
		return ((uint32_t)valBuffer[0] << 24) | ((uint32_t)valBuffer[1] << 16) | ((uint32_t)valBuffer[2] << 8) | (uint32_t)valBuffer[3];
	}
	uint64_t BinaryReadUint64BE(std::istream& stream) {
		// Read the value's bytes from the stream
		uint8_t valBuffer[8];
		stream.read((char*)valBuffer, 8);

		// Assemble the value
		return ((uint64_t)valBuffer[0] << 56) | ((uint64_t)valBuffer[1] << 48) | ((uint64_t)valBuffer[2] << 40) | ((uint64_t)valBuffer[3] << 32) | ((uint64_t)valBuffer[4] << 24) | ((uint64_t)valBuffer[5] << 16) | ((uint64_t)valBuffer[6] << 8) | (uint64_t)valBuffer[7];
	}
	float BinaryReadFloatBE(std::istream& stream) {
		// Read the equivalent IEEE encoding integer and convert it to a float
		uint32_t valInt = BinaryReadUint32BE(stream);
		return *(float*)&valInt;
	}
	double BinaryReadDoubleBE(std::istream& stream) {
		// Read the equivalent IEEE encoding integer and convert it to a double
		uint64_t valInt = BinaryReadUint64BE(stream);
		return *(double*)&valInt;
	}

	uint16_t BinaryReadUint16LE(std::istream& stream) {
		// Read the value's bytes from the stream
		uint8_t valBuffer[2];
		stream.read((char*)valBuffer, 2);

		// Assemble the value
		return (uint16_t)valBuffer[0] | ((uint16_t)valBuffer[1] << 8);
	}
	uint32_t BinaryReadUint32LE(std::istream& stream) {
		// Read the value's bytes from the stream
		uint8_t valBuffer[4];
		stream.read((char*)valBuffer, 4);

		// Assemble the value
		return (uint32_t)valBuffer[0] | ((uint32_t)valBuffer[1] << 8) | ((uint32_t)valBuffer[2] << 16) | ((uint32_t)valBuffer[3] << 24);
	}
	uint64_t BinaryReadUint64LE(std::istream& stream) {
		// Read the value's bytes from the stream
		uint8_t valBuffer[8];
		stream.read((char*)valBuffer, 8);

		// Assemble the value
		return (uint64_t)valBuffer[0] | ((uint64_t)valBuffer[1] << 8) | ((uint64_t)valBuffer[2] << 16) | ((uint64_t)valBuffer[3] << 24) | ((uint64_t)valBuffer[4] << 32) | ((uint64_t)valBuffer[5] << 40) | ((uint64_t)valBuffer[6] << 48) | ((uint64_t)valBuffer[7] << 56);
	}
	float BinaryReadFloatLE(std::istream& stream) {
		// Read the equivalent IEEE encoding integer and convert it to a float
		uint32_t valInt = BinaryReadUint32LE(stream);
		return *(float*)&valInt;
	}
	double BinaryReadDoubleLE(std::istream& stream) {
		// Read the equivalent IEEE encoding integer and convert it to a double
		uint64_t valInt = BinaryReadUint64LE(stream);
		return *(double*)&valInt;
	}

	void BinaryWriteUint16BE(std::ostream& stream, uint16_t val) {
		// Disassemble the value into its bytes
		uint8_t valBuffer[2];
		valBuffer[0] = (uint8_t)(val >> 8);
		valBuffer[1] = (uint8_t)val;

		// Write the value's bytes to the stream
		stream.write((const char*)valBuffer, 2);
	}
	void BinaryWriteUint32BE(std::ostream& stream, uint32_t val) {
		// Disassemble the value into its bytes
		uint8_t valBuffer[4];
		valBuffer[0] = (uint8_t)(val >> 24);
		valBuffer[1] = (uint8_t)(val >> 16);
		valBuffer[2] = (uint8_t)(val >> 8);
		valBuffer[3] = (uint8_t)val;

		// Write the value's bytes to the stream
		stream.write((const char*)valBuffer, 4);
	}
	void BinaryWriteUint64BE(std::ostream& stream, uint64_t val) {
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
	void BinaryWriteFloatBE(std::ostream& stream, float val) {
		// Write the float's IEEE encoding
		BinaryWriteUint32BE(stream, *(uint32_t*)&val);
	}
	void BinaryWriteDoubleBE(std::ostream& stream, double val) {
		// Write the double's IEEE encoding
		BinaryWriteUint64BE(stream, *(uint64_t*)&val);
	}

	void BinaryWriteUint16LE(std::ostream& stream, uint16_t val) {
		// Disassemble the value into its bytes
		uint8_t valBuffer[2];
		valBuffer[0] = (uint8_t)val;
		valBuffer[1] = (uint8_t)(val >> 8);

		// Write the value's bytes to the stream
		stream.write((const char*)valBuffer, 2);
	}
	void BinaryWriteUint32LE(std::ostream& stream, uint32_t val) {
		// Disassemble the value into its bytes
		uint8_t valBuffer[4];
		valBuffer[0] = (uint8_t)val;
		valBuffer[1] = (uint8_t)(val >> 8);
		valBuffer[2] = (uint8_t)(val >> 16);
		valBuffer[3] = (uint8_t)(val >> 24);

		// Write the value's bytes to the stream
		stream.write((const char*)valBuffer, 4);
	}
	void BinaryWriteUint64LE(std::ostream& stream, uint64_t val) {
		// Disassemble the value into its bytes
		uint8_t valBuffer[8];
		valBuffer[0] = (uint8_t)val;
		valBuffer[1] = (uint8_t)(val >> 8);
		valBuffer[2] = (uint8_t)(val >> 16);
		valBuffer[3] = (uint8_t)(val >> 24);
		valBuffer[4] = (uint8_t)(val >> 32);
		valBuffer[5] = (uint8_t)(val >> 40);
		valBuffer[6] = (uint8_t)(val >> 48);
		valBuffer[7] = (uint8_t)(val >> 56);

		// Write the value's bytes to the stream
		stream.write((const char*)valBuffer, 8);
	}
	void BinaryWriteFloatLE(std::ostream& stream, float val) {
		// Write the float's IEEE encoding
		BinaryWriteUint32LE(stream, *(uint32_t*)&val);
	}
	void BinaryWriteDoubleLE(std::ostream& stream, double val) {
		// Write the double's IEEE encoding
		BinaryWriteUint64LE(stream, *(uint64_t*)&val);
	}
}