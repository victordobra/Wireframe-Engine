#include "JSONParser.hpp"
#include <cstdlib>
#include <utility>

namespace wfe {
	// Internal helper functions
	static std::string ConvertTokenToString(const std::string& token) {
		// Parse all characters
		std::string res = "";

		for(size_t i = 1; i != token.size() - 1; ++i) {
			// Check if the current character is a special character
			if(token[i] == '\\') {
				// Check what the next character is
				++i;
				switch(token[i]) {
				case '\'':
					res.push_back('\'');
					break;
				case '\"':
					res.push_back('\"');
					break;
				case '\?':
					res.push_back('\?');
					break;
				case '\\':
					res.push_back('\\');
					break;
				case 'a':
					res.push_back('\a');
					break;
				case 'b':
					res.push_back('\b');
					break;
				case 'f':
					res.push_back('\f');
					break;
				case 'n':
					res.push_back('\n');
					break;
				case 'r':
					res.push_back('\r');
					break;
				case 't':
					res.push_back('\t');
					break;
				case 'v':
					res.push_back('\v');
					break;
				default:
					res.push_back(token[i]);
					break;
				}
			} else {
				// Add the current character
				res.push_back(token[i]);
			}
		}

		return res;
	}
	static std::string ConvertStringToToken(const std::string& str) {
		// Parse all characters in the string
		std::string res = "\"";

		for(size_t i = 0; i != str.size(); ++i) {
			// Add the current character to the token
			switch(str[i]) {
			case '\'':
				res.push_back('\\');
				res.push_back('\'');
				break;
			case '\"':
				res.push_back('\\');
				res.push_back('\"');
				break;
			case '\?':
				res.push_back('\\');
				res.push_back('\?');
				break;
			case '\\':
				res.push_back('\\');
				res.push_back('\\');
				break;
			case '\a':
				res.push_back('\\');
				res.push_back('\a');
				break;
			case '\b':
				res.push_back('\\');
				res.push_back('\b');
				break;
			case '\f':
				res.push_back('\\');
				res.push_back('\f');
				break;
			case '\n':
				res.push_back('\\');
				res.push_back('\n');
				break;
			case '\r':
				res.push_back('\\');
				res.push_back('\r');
				break;
			case '\t':
				res.push_back('\\');
				res.push_back('\t');
				break;
			case '\v':
				res.push_back('\\');
				res.push_back('\v');
				break;
			default:
				res.push_back(str[i]);
				break;
			}
		}

		// Add the end of the token and return it
		res.push_back('\"');
		return res;
	}

	bool JSONObject::InternalReadObject(const std::vector<std::string>& tokens, size_t& tokenIndex) {
		// Check if the first token is an opening bracket for the object
		if(tokens[tokenIndex] != "{")
			return false;
		++tokenIndex;

		// Check if the object is simply empty
		if(tokenIndex != tokens.size() && tokens[tokenIndex] == "}")
			return true;

		// Keep reading tokens until the end of the object is reached
		while(true) {
			// Read the current value's name
			if(tokenIndex == tokens.size() || tokens[tokenIndex][0] != '\"')
				return false;
			
			std::string name = ConvertTokenToString(tokens[tokenIndex]);

			// Check if the next token is ":"
			++tokenIndex;
			if(tokenIndex == tokens.size() || tokens[tokenIndex] != ":")
				return false;

			// Read the value
			++tokenIndex;
			if(tokenIndex == tokens.size())
				return false;
			
			char first = tokens[tokenIndex][0];
			switch(first) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '+':
			case '-':
			case '.':
				// Check if the current value is an integer or a float
				if(tokens[tokenIndex].find_first_of(".Ee") != std::string::npos) {
					// Try to convert the token string to a float value
					char* end;
					double value = std::strtod(tokens[tokenIndex].c_str(), &end);
					if(end != tokens[tokenIndex].c_str() + tokens[tokenIndex].size())
						return false;

					// Add the float value to the object
					if(!AddFloatValue(name, value))
						return false;
				} else {
					// Try to convert the token string to an int value
					char* end;
					int64_t value = (int64_t)std::strtoll(tokens[tokenIndex].c_str(), &end, 10);
					if(end != tokens[tokenIndex].c_str() + tokens[tokenIndex].size())
						return false;
						
					// Add the int value to the object
					if(!AddIntValue(name, value))
						return false;
				}

				++tokenIndex;
				break;
			case 't':
				// Add the true value to the object
				if(!AddBoolValue(name, true))
					return false;

				++tokenIndex;
				break;
			case 'f':
				// Add the false value to the object
				if(!AddBoolValue(name, false))
					return false;

				++tokenIndex;
				break;
			case '\"':
				// Add the string value to the object
				if(!AddStringValue(name, ConvertTokenToString(tokens[tokenIndex])))
					return false;

				++tokenIndex;
				break;
			case '{': {
				// Parse the child object
				JSONObject value;
				if(!value.InternalReadObject(tokens, tokenIndex))
					return false;
				
				// Add the object value to the object
				if(!AddJSONObjectValue(name, std::move(value)))
					return false;

				break;
			}
			case '[': {
				// Parse the child array
				JSONArray value;
				if(!value.InternalReadArray(tokens, tokenIndex))
					return false;
				
				// Add the array value to the object
				if(!AddJSONArrayValue(name, std::move(value)))
					return false;

				break;
			}
			case 'n':
				// Add the null value to the object
				if(!AddNullValue(name))
					return false;

				++tokenIndex;
				break;
			default:
				// An undefined token was found; exit the function
				return false;
			}

			// Read the next token and check if the object will end
			if(tokenIndex == tokens.size())
				return false;
			
			if(tokens[tokenIndex] == "}") {
				break;
			} else if(tokens[tokenIndex] != ",") {
				return false;
			}
			++tokenIndex;
		}

		++tokenIndex;
		return true;
	}
	void JSONObject::InternalWriteObject(std::ostream& stream, size_t tabCount) const {
		// Check if the object is empty
		if(values.empty()) {
			stream << "{ }";
			return;
		}
		
		// Write the object's opening bracket
		stream << "{\n";

		// Write every value
		size_t index = 0;
		for(const std::pair<std::string, Value>& value : values) {
			// Write the tabs starting the line
			for(size_t i = 0; i != tabCount + 1; ++i)
				stream << '\t';
			
			// Write the values name and the ":" token
			stream << ConvertStringToToken(value.first) << ": ";

			// Write the value
			switch(value.second.type) {
			case VALUE_TYPE_INT:
				// Write the integer
				stream << value.second.intValue;
				break;
			case VALUE_TYPE_FLOAT:
				// Write the float
				stream << value.second.floatValue;
				break;
			case VALUE_TYPE_BOOL:
				// Write the string corresponding to the boolean value
				if(value.second.boolValue) {
					stream << "true";
				} else {
					stream << "false";
				}
				break;
			case VALUE_TYPE_STRING:
				// Write the token corresponding to the string value
				stream << ConvertStringToToken(stringValues[value.second.stringIndex]);
				break;
			case VALUE_TYPE_OBJECT:
				// Write the object to the stream, indented by one more tab
				objectValues[value.second.objectIndex].InternalWriteObject(stream, tabCount + 1);
				break;
			case VALUE_TYPE_ARRAY:
				// Write the array to the stream, indented by one more tab
				arrayValues[value.second.arrayIndex].InternalWriteArray(stream, tabCount + 1);
				break;
			case VALUE_TYPE_NULL:
				// Write the null value
				stream << "null";
				break;
			}

			// Write a comma, if required, and a line break
			if(index != values.size() - 1)
				stream << ',';
			stream << '\n';

			// Update the index
			++index;
		}

		// Write the closing bracket
		for(size_t i = 0; i != tabCount; ++i)
			stream << '\t';
		stream << '}';
	}
	bool JSONArray::InternalReadArray(const std::vector<std::string>& tokens, size_t& tokenIndex) {
		// Check if the first token is an opening bracket for the array
		if(tokens[tokenIndex] != "[")
			return false;
		++tokenIndex;

		// Check if the array is simply empty
		if(tokenIndex != tokens.size() && tokens[tokenIndex] == "]") {
			++tokenIndex;
			return true;
		}

		// Keep reading tokens until the end of the array is reached
		while(true) {
			// Read the value
			if(tokenIndex == tokens.size())
				return false;
			
			char first = tokens[tokenIndex][0];
			switch(first) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '+':
			case '-':
			case '.':
				// Check if the current value is an integer or a float
				if(tokens[tokenIndex].find_first_of(".Ee") != std::string::npos) {
					// Try to convert the token string to a float value
					char* end;
					double value = std::strtod(tokens[tokenIndex].c_str(), &end);
					if(end != tokens[tokenIndex].c_str() + tokens[tokenIndex].size())
						return false;

					// Add the float value to the array
					AddFloatValue(value);
				} else {
					// Try to convert the token string to an int value
					char* end;
					int64_t value = (int64_t)std::strtoll(tokens[tokenIndex].c_str(), &end, 10);
					if(end != tokens[tokenIndex].c_str() + tokens[tokenIndex].size())
						return false;
						
					// Add the int value to the array
					AddIntValue(value);
				}

				++tokenIndex;
				break;
			case 't':
				// Add the true value to the array
				AddBoolValue(true);

				++tokenIndex;
				break;
			case 'f':
				// Add the false value to the array
				AddBoolValue(false);

				++tokenIndex;
				break;
			case '\"':
				// Add the string value to the array
				AddStringValue(ConvertTokenToString(tokens[tokenIndex]));

				++tokenIndex;
				break;
			case '{': {
				// Parse the child object
				JSONObject value;
				if(!value.InternalReadObject(tokens, tokenIndex))
					return false;
				
				// Add the object value to the array
				AddJSONObjectValue(std::move(value));

				break;
			}
			case '[': {
				// Parse the child array
				JSONArray value;
				if(!value.InternalReadArray(tokens, tokenIndex))
					return false;
				
				// Add the array value to the array
				AddJSONArrayValue(std::move(value));

				break;
			}
			case 'n':
				// Add the null value to the array
				AddNullValue();

				++tokenIndex;
				break;
			default:
				// An undefined token was found; exit the function
				return false;
			}

			// Read the next token and check if the array will end
			if(tokenIndex == tokens.size())
				return false;
			
			if(tokens[tokenIndex] == "]") {
				break;
			} else if(tokens[tokenIndex] != ",") {
				return false;
			}
			++tokenIndex;
		}

		++tokenIndex;
		return true;
	}
	void JSONArray::InternalWriteArray(std::ostream& stream, size_t tabCount) const {
		// Check if the array is empty
		if(values.empty()) {
			stream << "[ ]";
			return;
		}
		
		// Write the object's opening bracket
		stream << "[\n";

		// Write every value
		size_t index = 0;
		for(const JSONObject::Value& value : values) {
			// Write the tabs starting the line
			for(size_t i = 0; i != tabCount + 1; ++i)
				stream << '\t';
			
			// Write the value
			switch(value.type) {
			case JSONObject::VALUE_TYPE_INT:
				// Write the integer
				stream << value.intValue;
				break;
			case JSONObject::VALUE_TYPE_FLOAT:
				// Write the float
				stream << value.floatValue;
				break;
			case JSONObject::VALUE_TYPE_BOOL:
				// Write the string corresponding to the boolean value
				if(value.boolValue) {
					stream << "true";
				} else {
					stream << "false";
				}
				break;
			case JSONObject::VALUE_TYPE_STRING:
				// Write the token corresponding to the string value
				stream << ConvertStringToToken(stringValues[value.stringIndex]);
				break;
			case JSONObject::VALUE_TYPE_OBJECT:
				// Write the object to the stream, indented by one more tab
				objectValues[value.objectIndex].InternalWriteObject(stream, tabCount + 1);
				break;
			case JSONObject::VALUE_TYPE_ARRAY:
				// Write the array to the stream, indented by one more tab
				arrayValues[value.arrayIndex].InternalWriteArray(stream, tabCount + 1);
				break;
			case JSONObject::VALUE_TYPE_NULL:
				// Write the null value
				stream << "null";
				break;
			}

			// Write a comma, if required, and a line break
			if(index != values.size() - 1)
				stream << ',';
			stream << '\n';

			// Update the index
			++index;
		}

		// Write the closing bracket
		for(size_t i = 0; i != tabCount; ++i)
			stream << '\t';
		stream << ']';
	}

	// Public functions
	bool JSONObject::ReadObject(std::istream& stream) {
		// Tokenize the given input stream
		std::vector<std::string> tokens;

		while(stream) {
			// Read the current character
			char first;
			if(!stream.read(&first, 1))
				break;

			switch(first) {
			case '\"': {
				// The current token is a string; read all characters until the end of the string
				std::string token = "\"";

				for(char ch; stream.read(&ch, 1);) {
					// Add the current character to the token
					token.push_back(ch);

					// Exit the loop if the end of the string was found
					if(ch == '\"')
						break;

					// Add the next character as-is, if the current character is a backslash
					if(ch == '\\')
						token.push_back((char)stream.get());
				}

				// Check if the string read was valid
				if(token.size() == 1 || token.back() != '\"')
					return false;
				
				// Add the string to the token vector
				tokens.emplace_back(std::move(token));
				break;
			}
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '+':
			case '-':
			case '.': {
				// The current token is an integer; read all characters belonging to the number
				std::string token{ first };

				for(char ch; stream.read(&ch, 1);) {
					// Check if the current character is not part of the number
					if((ch < '0' || ch > '9') && ch != '.' && ch != '+' && ch != '-' && ch != 'e' && ch != 'E') {
						// Unget this character and exit the loop
						stream.unget();
						break;
					}

					// Add the current character to the token
					token.push_back(ch);
				}

				// Add the number to the token vector
				tokens.emplace_back(std::move(token));
				break;
			}
			case 't': {
				// The current token might be "true"; check if that is the case
				char rest[3];
				if(stream.read(rest, 3).gcount() != 3)
					return false;
				
				if(rest[0] != 'r' || rest[1] != 'u' || rest[2] != 'e')
					return false;
				
				// Add the true value to the token vector
				tokens.emplace_back("true");
				break;
			}
			case 'f': {
				// The current token might be "false"; check if that is the case
				char rest[4];
				if(stream.read(rest, 4).gcount() != 4)
					return false;
				
				if(rest[0] != 'a' || rest[1] != 'l' || rest[2] != 's' || rest[3] != 'e')
					return false;
				
				// Add the false value to the token vector
				tokens.emplace_back("false");
				break;
			}
			case 'n': {
				// The current token might be "null"; check if that is the case
				char rest[3];
				if(stream.read(rest, 3).gcount() != 3)
					return false;
				
				if(rest[0] != 'u' || rest[1] != 'l' || rest[2] != 'l')
					return false;
				
				// Add the null value to the token vector
				tokens.emplace_back("null");
				break;
			}
			case '{':
			case '}':
			case '[':
			case ']':
			case ':':
			case ',':
				// The current token is a structural character; add it to the token vector as-is
				tokens.emplace_back(1, first);
				break;
			case ' ':
			case '\f':
			case '\n':
			case '\r':
			case '\t':
			case '\v':
				// The current character is whitespace; ignore it
				break;
			default:
				// The current character should not be the first character in a token; exit the function
				return false;
			}
		}

		// Parse the object using the token vector
		size_t tokenIndex = 0;
		if(!InternalReadObject(tokens, tokenIndex))
			return false;
		if(tokenIndex != tokens.size())
			return false;
		
		return true;
	}
	void JSONObject::WriteObject(std::ostream& stream) const {
		// Write the object to the stream with no leading tabs
		InternalWriteObject(stream, 0);
	}
}