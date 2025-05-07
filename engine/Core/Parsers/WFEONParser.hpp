#pragma once

#include "Core/Types/Defines.hpp"
#include <istream>
#include <map>
#include <string>
#include <vector>

namespace wfe {
	/// @brief A struct that represents an object present in a WFEON file.
	struct WFEONObject {
	public:
		/// @brief An enum that represents the types of values stored in the object.
		enum ValueType {
			/// @brief Integer value type.
			VALUE_TYPE_INT,
			/// @brief Floating point value type.
			VALUE_TYPE_FLOAT,
			/// @brief Boolean value type.
			VALUE_TYPE_BOOL,
			/// @brief String value type.
			VALUE_TYPE_STRING,
			/// @brief Complex object value type.
			VALUE_TYPE_OBJECT,
			/// @brief The number of value types.
			VALUE_TYPE_COUNT
		};
		/// @brief A struct that represents a value stored in the object.
		struct WFEONValue {
			/// @brief The value's type.
			ValueType type;
			/// @brief The number of elements in the value. Different to 1 for arrays.
			size_t count;
			/// @brief The first index of the values in the object's vectors
			size_t startIndex;
		};

		/// @brief Parses a WFEON file and loads its into into this object.
		/// @param stream The stream to read the WFEON file from.
		void Parse(std::istream& stream);
		/// @brief Write the WFEON object to a stream.
		/// @param stream The stream to write the WFEON object to.
		/// @param floatPrecision The number of decimal places to write for floating point values.
		void Write(std::ostream& stream, size_t floatPrecision = 3) const;
	
		/// @brief A map of values stored in the object, keyed by their name.
		std::map<std::string, WFEONValue> values;
		/// @brief A vector containing all integer values stored in the object.
		std::vector<int64_t> intValues;
		/// @brief A vector containing all floating point values stored in the object.
		std::vector<float32_t> floatValues;
		/// @brief A vector containing all boolean values stored in the object.
		std::vector<bool8_t> boolValues;
		/// @brief A vector containing all string values stored in the object.
		std::vector<std::string> stringValues;
		/// @brief A vector containing all complex object values stored in the object.
		std::vector<WFEONObject> objectValues;
	private:
		void InternalParse(const std::vector<std::string>& tokens, size_t& index, bool root);
		void InternalWrite(std::ostream& stream, const std::string& prefix) const;
	};
}