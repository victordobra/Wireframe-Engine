#pragma once

#include "Core/Types/Defines.hpp"
#include <istream>
#include <ostream>

namespace wfe {
	/// @brief Reads a uint16_t from the given binary stream, using big endian encoding.
	/// @param stream The stream to read from.
	/// @return The uint16_t value read from the stream.
	uint16_t BinaryReadUint16(std::istream& stream);
	/// @brief Reads a uint32_t from the given binary stream, using big endian encoding.
	/// @param stream The stream to read from.
	/// @return The uint32_t value read from the stream.
	uint32_t BinaryReadUint32(std::istream& stream);
	/// @brief Reads a uint64_t from the given binary stream, using big endian encoding.
	/// @param stream The stream to read from.
	/// @return The uint64_t value read from the stream.
	uint64_t BinaryReadUint64(std::istream& stream);
	/// @brief Reads a float from the given binary stream, using big endian encoding.
	/// @param stream The stream to read from.
	/// @return The float value read from the stream.
	float BinaryReadFloat(std::istream& stream);
	/// @brief Reads a double from the given binary stream, using big endian encoding.
	/// @param stream The stream to read from.
	/// @return The double value read from the stream.
	double BinaryReadDouble(std::istream& stream);

	/// @brief Writes a uint16_t to the given binary stream, using big endian encoding.
	/// @param stream The stream to write to.
	/// @param val The uint16_t value write to the stream.
	void BinaryWriteUint16(std::ostream& stream, uint16_t val);
	/// @brief Writes a uint32_t to the given binary stream, using big endian encoding.
	/// @param stream The stream to write to.
	/// @param val The uint32_t value write to the stream.
	void BinaryWriteUint32(std::ostream& stream, uint32_t val);
	/// @brief Writes a uint64_t to the given binary stream, using big endian encoding.
	/// @param stream The stream to write to.
	/// @param val The uint64_t value write to the stream.
	void BinaryWriteUint64(std::ostream& stream, uint64_t val);
	/// @brief Writes a float to the given binary stream, using big endian encoding.
	/// @param stream The stream to write to.
	/// @param val The float value write to the stream.
	void BinaryWriteFloat(std::ostream& stream, float val);
	/// @brief Writes a double to the given binary stream, using big endian encoding.
	/// @param stream The stream to write to.
	/// @param val The double value write to the stream.
	void BinaryWriteDouble(std::ostream& stream, double val);
}