#include "WFEONParser.hpp"
#include <iomanip>

namespace wfe {
	// Internal helper functions
	void WFEONObject::InternalParse(const std::vector<std::string>& tokens, size_t& index, bool root) {
		// Keep loading until the end of the file or the end of the object
		while(index < tokens.size()) {
			// Load the variable name
			std::string name = tokens.at(index++);

			// Exit if the end of the object is reached
			if(name == "}") {
				if(root) {
					throw std::runtime_error("Error parsing WFEON file! Expected variable name, but found '}'.");
				} else {
					--index;
					return;
				}
			}

			// The next token should be an equals sign
			if(tokens.at(index++) != "=") {
				throw std::runtime_error("Error parsing WFEON file! Expected '=', but found '" + tokens.at(index - 1) + "'.");
			}

			// Check if the value is an array
			size_t maxCount = 1;
			if(tokens.at(index) == "[") {
				// Skip the opening bracket
				++index;

				// Set the max count to a very large number
				maxCount = SIZE_T_MAX;
			}

			// Keep reading values until reaching the end of array (if reading an array)
			size_t count = 0;
			ValueType valueType = VALUE_TYPE_COUNT;

			for(; count != maxCount; ++count) {
				// Check if the end of the array is reached
				if(tokens.at(index) == "]") {
					// Skip the closing bracket
					++index;
					break;
				}

				// Read the next value
				if(tokens.at(index)[0] == '"') {
					// Throw an error if the previous values in the array are not strings
					if(valueType != VALUE_TYPE_COUNT && valueType != VALUE_TYPE_STRING)
						throw std::runtime_error("Error parsing WFEON file! Expected string, but found '" + tokens.at(index) + "'.");
					valueType = VALUE_TYPE_STRING;

					// Loop through the string and format all special characters
					std::string str = "";
					for(size_t i = 1; i != tokens.at(index).size() - 1; ++i) {
						// Check if the next character is special
						if(tokens.at(index)[i] == '\\') {
							// Add the next character to the string
							++i;
							switch(tokens.at(index)[i]) {
							case '\'':
								str += '\'';
								break;
							case '\"':
								str += '\"';
								break;
							case '\?':
								str += '\?';
								break;
							case '\\':
								str += '\\';
								break;
							case 'a':
								str += '\a';
								break;
							case 'b':
								str += '\b';
								break;
							case 'f':
								str += '\f';
								break;
							case 'n':
								str += '\n';
								break;
							case 'r':
								str += '\r';
								break;
							case 't':
								str += '\t';
								break;
							case 'v':
								str += '\v';
								break;
							}
						} else {
							// Add the character to the string
							str += tokens.at(index)[i];
						}
					}

					// Append the string to its array
					this->stringValues.push_back(str);
				} else if(tokens.at(index) == "true" || tokens.at(index) == "false") {
					// Throw an error if the previous values in the array are not booleans
					if(valueType != VALUE_TYPE_COUNT && valueType != VALUE_TYPE_BOOL)
						throw std::runtime_error("Error parsing WFEON file! Expected boolean, but found '" + tokens.at(index) + "'.");
					valueType = VALUE_TYPE_BOOL;

					// Append the boolean to its array
					this->boolValues.push_back(tokens.at(index) == "true");
				} else if(std::isdigit(tokens.at(index)[0]) || tokens.at(index)[0] == '-') {
					// Check if the number is a float or an int
					if(tokens.at(index).find('.') != std::string::npos) {
						// Throw an error if the previous values in the array are not floats
						if(valueType != VALUE_TYPE_COUNT && valueType != VALUE_TYPE_FLOAT)
							throw std::runtime_error("Error parsing WFEON file! Expected float, but found '" + tokens.at(index) + "'.");
						valueType = VALUE_TYPE_FLOAT;

						// Append the float to its array
						this->floatValues.push_back(std::stof(tokens.at(index)));
					} else {
						// Throw an error if the previous values in the array are not ints
						if(valueType != VALUE_TYPE_COUNT && valueType != VALUE_TYPE_INT)
							throw std::runtime_error("Error parsing WFEON file! Expected int, but found '" + tokens.at(index) + "'.");
						valueType = VALUE_TYPE_INT;

						// Append the int to its array
						this->intValues.push_back(std::stoll(tokens.at(index)));
					}
				} else if(tokens.at(index) == "{") {
					// Throw an error if the previous values in the array are not objects
					if(valueType != VALUE_TYPE_COUNT && valueType != VALUE_TYPE_OBJECT)
						throw std::runtime_error("Error parsing WFEON file! Expected object, but found '" + tokens.at(index) + "'.");
					valueType = VALUE_TYPE_OBJECT;

					// Skip the opening bracket
					++index; 

					// Create a new object and parse it recursively
					WFEONObject newObject;
					newObject.InternalParse(tokens, index, false);
					
					// Append the object to its array
					this->objectValues.push_back(newObject);
				} else {
					// Throw an error if the value is not a valid type
					throw std::runtime_error("Error parsing WFEON file! Expected value, but found '" + tokens.at(index) + "'.");
				}

				++index;
			}

			// Get the total number of values in the array
			size_t totalCount = 0;
			switch(valueType) {
			case VALUE_TYPE_INT:
				totalCount = this->intValues.size();
				break;
			case VALUE_TYPE_FLOAT:
				totalCount = this->floatValues.size();
				break;
			case VALUE_TYPE_BOOL:
				totalCount = this->boolValues.size();
				break;
			case VALUE_TYPE_STRING:
				totalCount = this->stringValues.size();
				break;
			case VALUE_TYPE_OBJECT:
				totalCount = this->objectValues.size();
				break;
			}

			// Write the new value
			WFEONValue value {
				.type = valueType,
				.count = count,
				.startIndex = totalCount - count
			};
			if(this->values.insert({name, value}).second == false)
				throw std::runtime_error("Error parsing WFEON file! Duplicate variable name '" + name + "'.");
		}
	}
	void WFEONObject::InternalWrite(std::ostream& stream, const std::string& prefix) const {
		// Write all the values in the object
		for(const auto& value : this->values) {
			// Write the variable name
			stream << prefix << value.first << " = ";

			// Begin an array bracket if the value is an array
			if(value.second.count != 1)
				stream << "[ ";
			
			// Write all values
			switch(value.second.type) {
			case VALUE_TYPE_INT:
				for(size_t i = 0; i != value.second.count; ++i)
					stream << this->intValues[value.second.startIndex + i] << ' ';
				break;
			case VALUE_TYPE_FLOAT:
				for(size_t i = 0; i != value.second.count; ++i)
					stream << this->floatValues[value.second.startIndex + i] << ' ';
				break;
			case VALUE_TYPE_BOOL:
				for(size_t i = 0; i != value.second.count; ++i)
					stream << (this->boolValues[value.second.startIndex + i] ? "true" : "false") << ' ';
				break;
			case VALUE_TYPE_STRING:
				for(size_t i = 0; i != value.second.count; ++i) {
					// Build the result string
					std::string str = "";

					for(size_t j = 0; j != this->stringValues[value.second.startIndex + i].size(); ++j) {
						// Add the following character, with considerations for special characters
						switch(this->stringValues[value.second.startIndex + i][j]) {
						case '\'':
							str += "\\'";
							break;
						case '\"':
							str += "\\\"";
							break;
						case '\?':
							str += "\\?";
							break;
						case '\\':
							str += "\\\\";
							break;
						case '\a':
							str += "\\a";
							break;
						case '\b':
							str += "\\b";
							break;
						case '\f':
							str += "\\f";
							break;
						case '\n':
							str += "\\n";
							break;
						case '\r':
							str += "\\r";
							break;
						case '\t':
							str += "\\t";
							break;
						case '\v':
							str += "\\v";
							break;
						default:
							str += this->stringValues[value.second.startIndex + i][j];
						}
					}

					stream << '\"' << str << "\" ";
				}
				break;
			case VALUE_TYPE_OBJECT:
				// Check if the current value is an array
				if(value.second.count != 1) {
					// Move on to the next line
					stream << '\n';

					// Write all objects
					for(size_t i = 0; i != value.second.count; ++i) {
						stream << prefix << "\t{\n";
						this->objectValues[value.second.startIndex + i].InternalWrite(stream, prefix + "\t\t");
						stream << prefix << "\t}\n";
					}

					// Leave space for the closing bracket
					stream << prefix;
				} else {
					// Write the object
					stream << "{\n";
					this->objectValues[value.second.startIndex].InternalWrite(stream, prefix + "\t");
					stream << prefix << "}\n";
				}

				break;
			}

			// Write the closing bracket if the value is an array
			if(value.second.count != 1)
				stream << ']';
			stream << '\n';
		}
	}

	// Public functions
	void WFEONObject::Parse(std::istream& stream) {
		// Get all the tokens from the stream
		std::vector<std::string> tokens;
		std::string token = "";
		bool readingString = false, readingLineComment = false, readingBlockComment = false;
		bool stringSpecial = false;

		for(char_t c, prev = 0; stream.get(c); prev = c) {
			if(readingString) {
				// Add the character to the token
				token += c;

				// Check if the string is finished
				if(c == '\"' && !stringSpecial) {
					readingString = false;
					tokens.push_back(token);
					token = "";
				}

				// Check if the next character is special
				if(c == '\\') {
					stringSpecial = !stringSpecial;
				} else {
					stringSpecial = false;
				}
			} else if(readingLineComment) {
				// Check if the comment is finished
				if(c == '\n')
					readingLineComment = false;
			} else if(readingBlockComment) {
				// Check if the comment is finished
				if(c == '/' && token == "*") {
					readingBlockComment = false;
					token = "";
				} else {
					token = c;
				}
			} else if(std::isspace(c)) {
				// Add the previous token to the vector, if it exists
				if(!token.empty()) {
					tokens.push_back(token);
					token = "";
				}
			} else if(std::isalnum(c) || c == '_' || c == '-' || c == '.') {
				// Insert the previous token if leftover from comment checking
				if(token == "/") {
					tokens.push_back(token);
					token = "";
				}

				// Add the character to the token
				token += c;
			} else if(c == '"') {
				// Insert the previous token, if it exists
				if(!token.empty())
					tokens.push_back(token);
				token = "\"";

				// Start reading a string
				readingString = true;
			}else if(c == '/') {
				// Check if a line comment is being started
				if(token == "/") {
					readingLineComment = true;
					token = "";
				} else {
					// Insert the previous token, if it exists
					if(!token.empty())
						tokens.push_back(token);
					token = "/";
				}
			} else if(c == '*') {
				// Check if a block comment is being started
				if(token == "/") {
					readingBlockComment = true;
					token = "";
				} else {
					// Insert the previous token, if it exists
					if(!token.empty()) {
						tokens.push_back(token);
						token = "";
					}
					
					// Insert the current token
					tokens.push_back("*");
				}
			} else {
				// Insert the previous token, if it exists
				if(!token.empty()) {
					tokens.push_back(token);
					token = "";
				}
					
				// Insert the current token
				tokens.emplace_back(1, c);
			}
		}

		// Add the last token, if it exists
		if(!token.empty())
			tokens.push_back(token);
		
		// Load the object
		size_t index = 0;
		this->InternalParse(tokens, index, true);
	}
	void WFEONObject::Write(std::ostream& stream, size_t floatPrecision) const {
		// Get the stream's properties
		size_t oldPrecision = (size_t)stream.precision();
		std::ostream::fmtflags oldFlags = stream.flags();

		// Set the stream's properties
		stream << std::fixed << std::setprecision(floatPrecision);

		// Write the object to the stream
		this->InternalWrite(stream, "");

		// Restore the stream's properties
		stream.precision(oldPrecision);
		stream.flags(oldFlags);
	}
}