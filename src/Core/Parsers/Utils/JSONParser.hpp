#pragma once

#include "Core/Types/Defines.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace wfe {
	class JSONObject;
	class JSONArray;

	/// @brief A class representing a JSON object.
	class JSONObject {
	public:
		/// @brief An enum representing all possible types of JSON values.
		enum ValueType {
			/// @brief The integer value type.
			VALUE_TYPE_INT,
			/// @brief The floating point value type.
			VALUE_TYPE_FLOAT,
			/// @brief The boolean value type.
			VALUE_TYPE_BOOL,
			/// @brief The string value type.
			VALUE_TYPE_STRING,
			/// @brief The JSON object value type.
			VALUE_TYPE_OBJECT,
			/// @brief The JSON array value type.
			VALUE_TYPE_ARRAY,
			/// @brief The null value type.
			VALUE_TYPE_NULL
		};

		/// @brief Creates an empty JSON object.
		JSONObject() = default;
		/// @brief Copies the given JSON object.
		/// @param other The JSON object to copy.
		JSONObject(const JSONObject& other) = default;
		/// @brief Moves the given JSON object.
		/// @param other The JSON object to move.
		JSONObject(JSONObject&& other) noexcept = default;

		/// @brief Copies the given JSON object's data into this JSON object.
		/// @param other The JSON object to copy.
		/// @return A reference to this JSON object.
		JSONObject& operator=(const JSONObject& other) = default;
		/// @brief Moves the given JSON object's data into this JSON object.
		/// @param other The JSON object to copy.
		/// @return A reference to this JSON object.
		JSONObject& operator=(JSONObject&& other) = default;

		/// @brief Gets the number of values in the object.
		/// @return The number of values in the object.
		size_t GetValueCount() const {
			return values.size();
		}
		/// @brief Checks if the object has the given value.
		/// @param name The name of the requested value.
		/// @return True if the object has a value with the given name, otherwise false.
		bool HasValue(const std::string& name) const {
			return values.count(name);
		}
		/// @brief Gets the type of the value with the given name.
		/// @param name The name of the value.
		/// @return The type of the value.
		ValueType GetValueType(const std::string& name) const {
			return values.at(name).type;
		}

		/// @brief Gets the integer value with the given name.
		/// @param name The name of the value.
		/// @return A reference to the value.
		int64_t& GetIntValue(const std::string& name) {
			// Check if the value's type is correct
			Value& value = values.at(name);
			if(value.type != VALUE_TYPE_INT)
				throw std::invalid_argument("The requested JSON value is not an integer!");
			
			return value.intValue;
		}
		/// @brief Gets the integer value with the given name.
		/// @param name The name of the value.
		/// @return A const reference to the value.
		const int64_t& GetIntValue(const std::string& name) const {
			// Check if the value's type is correct
			const Value& value = values.at(name);
			if(value.type != VALUE_TYPE_INT)
				throw std::invalid_argument("The requested JSON value is not an integer!");
			
			return value.intValue;
		}
		/// @brief Gets the floating point value with the given name.
		/// @param name The name of the value.
		/// @return A reference to the value.
		double& GetFloatValue(const std::string& name) {
			// Check if the value's type is correct
			Value& value = values.at(name);
			if(value.type != VALUE_TYPE_FLOAT)
				throw std::invalid_argument("The requested JSON value is not a floating point number!");
			
			return value.floatValue;
		}
		/// @brief Gets the floating point value with the given name.
		/// @param name The name of the value.
		/// @return A const reference to the value.
		const double& GetFloatValue(const std::string& name) const {
			// Check if the value's type is correct
			const Value& value = values.at(name);
			if(value.type != VALUE_TYPE_INT)
				throw std::invalid_argument("The requested JSON value is not a floating point number!");
			
			return value.floatValue;
		}
		/// @brief Gets the boolean value with the given name.
		/// @param name The name of the value.
		/// @return A reference to the value.
		bool& GetBoolValue(const std::string& name) {
			// Check if the value's type is correct
			Value& value = values.at(name);
			if(value.type != VALUE_TYPE_BOOL)
				throw std::invalid_argument("The requested JSON value is not a boolean!");
			
			return value.boolValue;
		}
		/// @brief Gets the boolean value with the given name.
		/// @param name The name of the value.
		/// @return A const reference to the value.
		const bool& GetBoolValue(const std::string& name) const {
			// Check if the value's type is correct
			const Value& value = values.at(name);
			if(value.type != VALUE_TYPE_BOOL)
				throw std::invalid_argument("The requested JSON value is not a boolean!");
			
			return value.boolValue;
		}
		/// @brief Gets the string value with the given name.
		/// @param name The name of the value.
		/// @return A reference to the value.
		std::string& GetStringValue(const std::string& name) {
			// Check if the value's type is correct
			Value& value = values.at(name);
			if(value.type != VALUE_TYPE_STRING)
				throw std::invalid_argument("The requested JSON value is not a string!");
			
			return stringValues[value.stringIndex];
		}
		/// @brief Gets the string value with the given name.
		/// @param name The name of the value.
		/// @return A const reference to the value.
		const std::string& GetStringValue(const std::string& name) const {
			// Check if the value's type is correct
			const Value& value = values.at(name);
			if(value.type != VALUE_TYPE_STRING)
				throw std::invalid_argument("The requested JSON value is not a string!");
			
			return stringValues[value.stringIndex];
		}
		/// @brief Gets the JSON object value with the given name.
		/// @param name The name of the value.
		/// @return A reference to the value.
		JSONObject& GetJSONObjectValue(const std::string& name) {
			// Check if the value's type is correct
			Value& value = values.at(name);
			if(value.type != VALUE_TYPE_OBJECT)
				throw std::invalid_argument("The requested JSON value is not a JSON object!");
			
			return objectValues[value.objectIndex];
		}
		/// @brief Gets the JSON object value with the given name.
		/// @param name The name of the value.
		/// @return A const reference to the value.
		const JSONObject& GetJSONObjectValue(const std::string& name) const {
			// Check if the value's type is correct
			const Value& value = values.at(name);
			if(value.type != VALUE_TYPE_OBJECT)
				throw std::invalid_argument("The requested JSON value is not a JSON object!");
			
			return objectValues[value.objectIndex];
		}
		/// @brief Gets the JSON array value with the given name.
		/// @param name The name of the value.
		/// @return A reference to the value.
		JSONArray& GetJSONArrayValue(const std::string& name) {
			// Check if the value's type is correct
			Value& value = values.at(name);
			if(value.type != VALUE_TYPE_ARRAY)
				throw std::invalid_argument("The requested JSON value is not a JSON array!");
			
			return arrayValues[value.objectIndex];
		}
		/// @brief Gets the JSON array value with the given name.
		/// @param name The name of the value.
		/// @return A const reference to the value.
		const JSONArray& GetJSONArrayValue(const std::string& name) const {
			// Check if the value's type is correct
			const Value& value = values.at(name);
			if(value.type != VALUE_TYPE_ARRAY)
				throw std::invalid_argument("The requested JSON value is not a JSON array!");
			
			return arrayValues[value.objectIndex];
		}

		/// @brief Adds a new integer value to the JSON object.
		/// @param name The name of the value to add.
		/// @param val The value to add.
		/// @brief False if a value with the given name already exists, otherwise true.
		bool AddIntValue(const std::string& name, int64_t val) {
			// Create and insert the value
			Value value {
				.type = VALUE_TYPE_INT,
				.intValue = val
			};

			return values.insert({ name, value }).second;
		}
		/// @brief Adds a new floating point value to the JSON object.
		/// @param name The name of the value to add.
		/// @param val The value to add.
		/// @brief False if a value with the given name already exists, otherwise true.
		bool AddFloatValue(const std::string& name, double val) {
			// Create and insert the value
			Value value {
				.type = VALUE_TYPE_FLOAT,
				.floatValue = val
			};

			return values.insert({ name, value }).second;
		}
		/// @brief Adds a new boolean value to the JSON object.
		/// @param name The name of the value to add.
		/// @param val The value to add.
		/// @brief False if a value with the given name already exists, otherwise true.
		bool AddBoolValue(const std::string& name, bool val) {
			// Create and insert the value
			Value value {
				.type = VALUE_TYPE_BOOL,
				.boolValue = val
			};

			return values.insert({ name, value }).second;
		}
		/// @brief Adds a new string value to the JSON object.
		/// @param name The name of the value to add.
		/// @param val The value to add.
		/// @brief False if a value with the given name already exists, otherwise true.
		bool AddStringValue(const std::string& name, const std::string& val) {
			// Create and insert the value
			Value value {
				.type = VALUE_TYPE_STRING,
				.stringIndex = stringValues.size()
			};

			bool valid = values.insert({ name, value }).second;

			// Add the string to the string vector, if valid
			if(valid)
				stringValues.push_back(val);
			
			return valid;
		}
		/// @brief Adds a new string value to the JSON object.
		/// @param name The name of the value to add.
		/// @param val The value to add.
		/// @brief False if a value with the given name already exists, otherwise true.
		bool AddStringValue(const std::string& name, std::string&& val) {
			// Create and insert the value
			Value value {
				.type = VALUE_TYPE_STRING,
				.stringIndex = stringValues.size()
			};

			bool valid = values.insert({ name, value }).second;

			// Add the string to the string vector, if valid
			if(valid)
				stringValues.emplace_back(val);
			
			return valid;
		}
		/// @brief Adds a new JSON object value to the JSON object.
		/// @param name The name of the value to add.
		/// @param val The value to add.
		/// @brief False if a value with the given name already exists, otherwise true.
		bool AddJSONObjectValue(const std::string& name, const JSONObject& val) {
			// Create and insert the value
			Value value {
				.type = VALUE_TYPE_OBJECT,
				.objectIndex = objectValues.size()
			};

			bool valid = values.insert({ name, value }).second;

			// Add the object to the object vector, if valid
			if(valid)
				objectValues.push_back(val);
			
			return valid;
		}
		/// @brief Adds a new JSON object value to the JSON object.
		/// @param name The name of the value to add.
		/// @param val The value to add.
		/// @brief False if a value with the given name already exists, otherwise true.
		bool AddJSONObjectValue(const std::string& name, JSONObject&& val) {
			// Create and insert the value
			Value value {
				.type = VALUE_TYPE_OBJECT,
				.objectIndex = objectValues.size()
			};

			bool valid = values.insert({ name, value }).second;

			// Add the object to the object vector, if valid
			if(valid)
				objectValues.emplace_back(val);
			
			return valid;
		}
		/// @brief Adds a new JSON array value to the JSON object.
		/// @param name The name of the value to add.
		/// @param val The value to add.
		/// @brief False if a value with the given name already exists, otherwise true.
		bool AddJSONArrayValue(const std::string& name, const JSONArray& val) {
			// Create and insert the value
			Value value {
				.type = VALUE_TYPE_ARRAY,
				.arrayIndex = arrayValues.size()
			};

			bool valid = values.insert({ name, value }).second;

			// Add the array to the array vector, if valid
			if(valid)
				arrayValues.push_back(val);
			
			return valid;
		}
		/// @brief Adds a new JSON array value to the JSON object.
		/// @param name The name of the value to add.
		/// @param val The value to add.
		/// @brief False if a value with the given name already exists, otherwise true.
		bool AddJSONArrayValue(const std::string& name, JSONArray&& val) {
			// Create and insert the value
			Value value {
				.type = VALUE_TYPE_ARRAY,
				.arrayIndex = arrayValues.size()
			};

			bool valid = values.insert({ name, value }).second;

			// Add the array to the array vector, if valid
			if(valid)
				arrayValues.emplace_back(val);
			
			return valid;
		}
		/// @brief Adds a new null value to the JSON object.
		/// @param name The name of the value to add.
		/// @brief False if a value with the given name already exists, otherwise true.
		bool AddNullValue(const std::string& name) {
			// Create and insert the value
			Value value {
				.type = VALUE_TYPE_NULL
			};

			return values.insert({ name, value }).second;
		}

		/// @brief Reads this JSON object's data from an input stream.
		/// @param stream The stream to read the JSON object from.
		/// @return True if the read was successful, otherwise false.
		bool ReadObject(std::istream& stream);
		/// @brief Formats and writes this JSON object's data to an output stream.
		/// @param stream The stream to write the JSON object to.
		void WriteObject(std::ostream& stream) const;

		/// @brief Destroys this JSON object.
		~JSONObject() = default;
	private:
		bool InternalReadObject(const std::vector<std::string>& tokens, size_t& tokenIndex);
		void InternalWriteObject(std::ostream& stream, size_t tabCount) const;

		friend JSONArray;

		struct Value {
			ValueType type;

			union {
				int64_t intValue;
				double floatValue;
				bool boolValue;
				size_t stringIndex;
				size_t objectIndex;
				size_t arrayIndex;
			};
		};

		std::unordered_map<std::string, Value> values;

		std::vector<std::string> stringValues;
		std::vector<JSONObject> objectValues;
		std::vector<JSONArray> arrayValues;
	};
	/// @brief A class representing a JSON array.
	class JSONArray {
	public:
		/// @brief Creates an empty JSON array.
		JSONArray() = default;
		/// @brief Copies the given JSON array.
		/// @param other The JSON array to copy.
		JSONArray(const JSONArray& other) = default;
		/// @brief Moves the given JSON array.
		/// @param other The JSON array to move.
		JSONArray(JSONArray&& other) noexcept = default;

		/// @brief Copies the given JSON array's data into this JSON array.
		/// @param other The JSON array to copy.
		/// @return A reference to this JSON array.
		JSONArray& operator=(const JSONArray& other) = default;
		/// @brief Moves the given JSON array's data into this JSON array.
		/// @param other The JSON array to copy.
		/// @return A reference to this JSON array.
		JSONArray& operator=(JSONArray&& other) = default;

		/// @brief Gets the number of values in the array.
		/// @return The number of values in the array.
		size_t GetValueCount() const {
			return values.size();
		}
		/// @brief Gets the type of the value with the given index.
		/// @param indx The index of the value.
		/// @return The type of the value.
		JSONObject::ValueType GetValueType(size_t index) const {
			return values[index].type;
		}

		/// @brief Gets the integer value with the given index.
		/// @param index The index of the value.
		/// @return A reference to the value.
		int64_t& GetIntValue(size_t index) {
			// Check if the value's type is correct
			if(values[index].type != JSONObject::VALUE_TYPE_INT)
				throw std::invalid_argument("The requested JSON value is not an integer!");
			
			return values[index].intValue;
		}
		/// @brief Gets the integer value with the given index.
		/// @param index The index of the value.
		/// @return A const reference to the value.
		const int64_t& GetIntValue(size_t index) const {
			// Check if the value's type is correct
			if(values[index].type != JSONObject::VALUE_TYPE_INT)
				throw std::invalid_argument("The requested JSON value is not an integer!");
			
			return values[index].intValue;
		}
		/// @brief Gets the floating point value with the given index.
		/// @param index The index of the value.
		/// @return A reference to the value.
		double& GetFloatValue(size_t index) {
			// Check if the value's type is correct
			if(values[index].type != JSONObject::VALUE_TYPE_FLOAT)
				throw std::invalid_argument("The requested JSON value is not a floating point number!");
			
			return values[index].floatValue;
		}
		/// @brief Gets the floating point value with the given index.
		/// @param index The index of the value.
		/// @return A const reference to the value.
		const double& GetFloatValue(size_t index) const {
			// Check if the value's type is correct
			if(values[index].type != JSONObject::VALUE_TYPE_INT)
				throw std::invalid_argument("The requested JSON value is not a floating point number!");
			
			return values[index].floatValue;
		}
		/// @brief Gets the boolean value with the given index.
		/// @param index The index of the value.
		/// @return A reference to the value.
		bool& GetBoolValue(size_t index) {
			// Check if the value's type is correct
			if(values[index].type != JSONObject::VALUE_TYPE_BOOL)
				throw std::invalid_argument("The requested JSON value is not a boolean!");
			
			return values[index].boolValue;
		}
		/// @brief Gets the boolean value with the given index.
		/// @param index The index of the value.
		/// @return A const reference to the value.
		const bool& GetBoolValue(size_t index) const {
			// Check if the value's type is correct
			if(values[index].type != JSONObject::VALUE_TYPE_BOOL)
				throw std::invalid_argument("The requested JSON value is not a boolean!");
			
			return values[index].boolValue;
		}
		/// @brief Gets the string value with the given index.
		/// @param index The index of the value.
		/// @return A reference to the value.
		std::string& GetStringValue(size_t index) {
			// Check if the value's type is correct
			if(values[index].type != JSONObject::VALUE_TYPE_STRING)
				throw std::invalid_argument("The requested JSON value is not a string!");
			
			return stringValues[values[index].stringIndex];
		}
		/// @brief Gets the string value with the given index.
		/// @param index The index of the value.
		/// @return A const reference to the value.
		const std::string& GetStringValue(size_t index) const {
			// Check if the value's type is correct
			if(values[index].type != JSONObject::VALUE_TYPE_STRING)
				throw std::invalid_argument("The requested JSON value is not a string!");
			
			return stringValues[values[index].stringIndex];
		}
		/// @brief Gets the JSON object value with the given index.
		/// @param index The index of the value.
		/// @return A reference to the value.
		JSONObject& GetJSONObjectValue(size_t index) {
			// Check if the value's type is correct
			if(values[index].type != JSONObject::VALUE_TYPE_OBJECT)
				throw std::invalid_argument("The requested JSON value is not a JSON object!");
			
			return objectValues[values[index].objectIndex];
		}
		/// @brief Gets the JSON object value with the given index.
		/// @param index The index of the value.
		/// @return A const reference to the value.
		const JSONObject& GetJSONObjectValue(size_t index) const {
			// Check if the value's type is correct
			if(values[index].type != JSONObject::VALUE_TYPE_OBJECT)
				throw std::invalid_argument("The requested JSON value is not a JSON object!");
			
			return objectValues[values[index].objectIndex];
		}
		/// @brief Gets the JSON array value with the given index.
		/// @param index The index of the value.
		/// @return A reference to the value.
		JSONArray& GetJSONArrayValue(size_t index) {
			// Check if the value's type is correct
			if(values[index].type != JSONObject::VALUE_TYPE_ARRAY)
				throw std::invalid_argument("The requested JSON value is not a JSON array!");
			
			return arrayValues[values[index].objectIndex];
		}
		/// @brief Gets the JSON array value with the given index.
		/// @param index The index of the value.
		/// @return A const reference to the value.
		const JSONArray& GetJSONArrayValue(size_t index) const {
			// Check if the value's type is correct
			if(values[index].type != JSONObject::VALUE_TYPE_ARRAY)
				throw std::invalid_argument("The requested JSON value is not a JSON array!");
			
			return arrayValues[values[index].objectIndex];
		}

		/// @brief Adds a new integer value to the end of the JSON array.
		/// @param val The value to add.
		void AddIntValue(int64_t val) {
			// Create and insert the value
			JSONObject::Value value {
				.type = JSONObject::VALUE_TYPE_INT,
				.intValue = val
			};

			values.push_back(value);
		}
		/// @brief Adds a new floating point value to the end of the JSON array.
		/// @param val The value to add.
		void AddFloatValue(double val) {
			// Create and insert the value
			JSONObject::Value value {
				.type = JSONObject::VALUE_TYPE_FLOAT,
				.floatValue = val
			};

			values.push_back(value);
		}
		/// @brief Adds a new boolean value to the end of the JSON array.
		/// @param val The value to add.
		void AddBoolValue(bool val) {
			// Create and insert the value
			JSONObject::Value value {
				.type = JSONObject::VALUE_TYPE_BOOL,
				.boolValue = val
			};

			values.push_back(value);
		}
		/// @brief Adds a new string value to the end of the JSON array.
		/// @param val The value to add.
		void AddStringValue(const std::string& val) {
			// Create and insert the value
			JSONObject::Value value {
				.type = JSONObject::VALUE_TYPE_STRING,
				.stringIndex = stringValues.size()
			};

			values.push_back(value);

			// Insert the string value
			stringValues.push_back(val);
		}
		/// @brief Adds a new string value to the end of the JSON array.
		/// @param val The value to add.
		void AddStringValue(std::string&& val) {
			// Create and insert the value
			JSONObject::Value value {
				.type = JSONObject::VALUE_TYPE_STRING,
				.stringIndex = stringValues.size()
			};

			values.push_back(value);

			// Insert the string value
			stringValues.emplace_back(val);
		}
		/// @brief Adds a new JSON object value to the end of the JSON array.
		/// @param val The value to add.
		void AddJSONObjectValue(const JSONObject& val) {
			// Create and insert the value
			JSONObject::Value value {
				.type = JSONObject::VALUE_TYPE_OBJECT,
				.objectIndex = objectValues.size()
			};

			values.push_back(value);

			// Insert the object value
			objectValues.push_back(val);
		}
		/// @brief Adds a new JSON object value to the end of the JSON array.
		/// @param val The value to add.
		void AddJSONObjectValue(JSONObject&& val) {
			// Create and insert the value
			JSONObject::Value value {
				.type = JSONObject::VALUE_TYPE_OBJECT,
				.objectIndex = objectValues.size()
			};

			values.push_back(value);

			// Insert the object value
			objectValues.emplace_back(val);
		}
		/// @brief Adds a new JSON array value to the end of the JSON array.
		/// @param val The value to add.
		void AddJSONArrayValue(const JSONArray& val) {
			// Create and insert the value
			JSONObject::Value value {
				.type = JSONObject::VALUE_TYPE_ARRAY,
				.objectIndex = arrayValues.size()
			};

			values.push_back(value);

			// Insert the array value
			arrayValues.push_back(val);
		}
		/// @brief Adds a new JSON array value to the end of the JSON array.
		/// @param val The value to add.
		void AddJSONArrayValue(JSONArray&& val) {
			// Create and insert the value
			JSONObject::Value value {
				.type = JSONObject::VALUE_TYPE_ARRAY,
				.objectIndex = arrayValues.size()
			};

			values.push_back(value);

			// Insert the array value
			arrayValues.emplace_back(val);
		}
		/// @brief Adds a new null value to the end of the JSON array.
		void AddNullValue() {
			// Create and insert the value
			JSONObject::Value value {
				.type = JSONObject::VALUE_TYPE_NULL
			};

			values.push_back(value);
		}

		/// @brief Destroys this JSON array.
		~JSONArray() = default;
	private:
		bool InternalReadArray(const std::vector<std::string>& tokens, size_t& tokenIndex);
		void InternalWriteArray(std::ostream& stream, size_t tabCount) const;

		friend JSONObject;

		std::vector<JSONObject::Value> values;
		std::vector<std::string> stringValues;
		std::vector<JSONObject> objectValues;
		std::vector<JSONArray> arrayValues;
	};
}