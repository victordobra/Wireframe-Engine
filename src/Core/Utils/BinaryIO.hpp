#pragma once

#include "Core/Types/Defines.hpp"
#include <istream>
#include <ostream>

namespace wfe {
	/// @brief Reads a uint16_t from the given binary stream, using big endian encoding.
	/// @param stream The stream to read from.
	/// @return The uint16_t value read from the stream.
	uint16_t BinaryReadUint16BE(std::istream& stream);
	/// @brief Reads a uint32_t from the given binary stream, using big endian encoding.
	/// @param stream The stream to read from.
	/// @return The uint32_t value read from the stream.
	uint32_t BinaryReadUint32BE(std::istream& stream);
	/// @brief Reads a uint64_t from the given binary stream, using big endian encoding.
	/// @param stream The stream to read from.
	/// @return The uint64_t value read from the stream.
	uint64_t BinaryReadUint64BE(std::istream& stream);
	/// @brief Reads a float from the given binary stream, using big endian encoding.
	/// @param stream The stream to read from.
	/// @return The float value read from the stream.
	float BinaryReadFloatBE(std::istream& stream);
	/// @brief Reads a double from the given binary stream, using big endian encoding.
	/// @param stream The stream to read from.
	/// @return The double value read from the stream.
	double BinaryReadDoubleBE(std::istream& stream);

	/// @brief Reads a uint16_t from the given binary stream, using little endian encoding.
	/// @param stream The stream to read from.
	/// @return The uint16_t value read from the stream.
	uint16_t BinaryReadUint16LE(std::istream& stream);
	/// @brief Reads a uint32_t from the given binary stream, using little endian encoding.
	/// @param stream The stream to read from.
	/// @return The uint32_t value read from the stream.
	uint32_t BinaryReadUint32LE(std::istream& stream);
	/// @brief Reads a uint64_t from the given binary stream, using little endian encoding.
	/// @param stream The stream to read from.
	/// @return The uint64_t value read from the stream.
	uint64_t BinaryReadUint64LE(std::istream& stream);
	/// @brief Reads a float from the given binary stream, using little endian encoding.
	/// @param stream The stream to read from.
	/// @return The float value read from the stream.
	float BinaryReadFloatLE(std::istream& stream);
	/// @brief Reads a double from the given binary stream, using little endian encoding.
	/// @param stream The stream to read from.
	/// @return The double value read from the stream.
	double BinaryReadDoubleLE(std::istream& stream);

	/// @brief Writes a uint16_t to the given binary stream, using big endian encoding.
	/// @param stream The stream to write to.
	/// @param val The uint16_t value write to the stream.
	void BinaryWriteUint16BE(std::ostream& stream, uint16_t val);
	/// @brief Writes a uint32_t to the given binary stream, using big endian encoding.
	/// @param stream The stream to write to.
	/// @param val The uint32_t value write to the stream.
	void BinaryWriteUint32BE(std::ostream& stream, uint32_t val);
	/// @brief Writes a uint64_t to the given binary stream, using big endian encoding.
	/// @param stream The stream to write to.
	/// @param val The uint64_t value write to the stream.
	void BinaryWriteUint64BE(std::ostream& stream, uint64_t val);
	/// @brief Writes a float to the given binary stream, using big endian encoding.
	/// @param stream The stream to write to.
	/// @param val The float value write to the stream.
	void BinaryWriteFloatBE(std::ostream& stream, float val);
	/// @brief Writes a double to the given binary stream, using big endian encoding.
	/// @param stream The stream to write to.
	/// @param val The double value write to the stream.
	void BinaryWriteDoubleBE(std::ostream& stream, double val);

	/// @brief Writes a uint16_t to the given binary stream, using little endian encoding.
	/// @param stream The stream to write to.
	/// @param val The uint16_t value write to the stream.
	void BinaryWriteUint16LE(std::ostream& stream, uint16_t val);
	/// @brief Writes a uint32_t to the given binary stream, using little endian encoding.
	/// @param stream The stream to write to.
	/// @param val The uint32_t value write to the stream.
	void BinaryWriteUint32LE(std::ostream& stream, uint32_t val);
	/// @brief Writes a uint64_t to the given binary stream, using little endian encoding.
	/// @param stream The stream to write to.
	/// @param val The uint64_t value write to the stream.
	void BinaryWriteUint64LE(std::ostream& stream, uint64_t val);
	/// @brief Writes a float to the given binary stream, using little endian encoding.
	/// @param stream The stream to write to.
	/// @param val The float value write to the stream.
	void BinaryWriteFloatLE(std::ostream& stream, float val);
	/// @brief Writes a double to the given binary stream, using little endian encoding.
	/// @param stream The stream to write to.
	/// @param val The double value write to the stream.
	void BinaryWriteDoubleLE(std::ostream& stream, double val);
}