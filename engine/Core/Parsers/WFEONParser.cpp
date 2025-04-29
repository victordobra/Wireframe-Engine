#include "WFEONParser.hpp"
#include <iostream>

namespace wfe {
	// Internal helper functions
	void WFEONObject::InternalParse(const std::vector<std::string>& tokens, size_t& index, WFEONObject& object, bool root) {
		// Keep loading until the end of the file or the end of the object
		while(index < tokens.size()) {
			// Load the variable name
			std::string name = tokens.at(index++);

			// Exit if the end of the object is reached
			if(name == "}") {
				if(root) {
					throw std::runtime_error("Error parsing WFEON file! Expected variable name, but found '}'.");
				} else {
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
					object.stringValues.push_back(str);
				} else if(tokens.at(index) == "true" || tokens.at(index) == "false") {
					// Throw an error if the previous values in the array are not booleans
					if(valueType != VALUE_TYPE_COUNT && valueType != VALUE_TYPE_BOOL)
						throw std::runtime_error("Error parsing WFEON file! Expected boolean, but found '" + tokens.at(index) + "'.");
					valueType = VALUE_TYPE_BOOL;

					// Append the boolean to its array
					object.boolValues.push_back(tokens.at(index) == "true");
				} else if(std::isdigit(tokens.at(index)[0]) || tokens.at(index)[0] == '-') {
					// Check if the number is a float or an int
					if(tokens.at(index).find('.') != std::string::npos) {
						// Throw an error if the previous values in the array are not floats
						if(valueType != VALUE_TYPE_COUNT && valueType != VALUE_TYPE_FLOAT)
							throw std::runtime_error("Error parsing WFEON file! Expected float, but found '" + tokens.at(index) + "'.");
						valueType = VALUE_TYPE_FLOAT;

						// Append the float to its array
						object.floatValues.push_back(std::stof(tokens.at(index)));
					} else {
						// Throw an error if the previous values in the array are not ints
						if(valueType != VALUE_TYPE_COUNT && valueType != VALUE_TYPE_INT)
							throw std::runtime_error("Error parsing WFEON file! Expected int, but found '" + tokens.at(index) + "'.");
						valueType = VALUE_TYPE_INT;

						// Append the int to its array
						object.intValues.push_back(std::stoll(tokens.at(index)));
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
					InternalParse(tokens, index, newObject, false);
					
					// Append the object to its array
					object.objectValues.push_back(newObject);
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
				totalCount = object.intValues.size();
				break;
			case VALUE_TYPE_FLOAT:
				totalCount = object.floatValues.size();
				break;
			case VALUE_TYPE_BOOL:
				totalCount = object.boolValues.size();
				break;
			case VALUE_TYPE_STRING:
				totalCount = object.stringValues.size();
				break;
			case VALUE_TYPE_OBJECT:
				totalCount = object.objectValues.size();
				break;
			}

			// Write the new value
			WFEONValue value {
				.type = valueType,
				.count = count,
				.startIndex = totalCount - count
			};
			object.values[name] = value;
		}
	}

	// Public functions
	WFEONObject WFEONObject::Parse(std::istream& stream) {
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
		
		// Load the root object
		WFEONObject rootObject;
		size_t index = 0;
		InternalParse(tokens, index, rootObject, true);
		return rootObject;
	}
}