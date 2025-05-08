#pragma once

#include "Core/Types/Defines.hpp"
#include <fstream>
#include <iostream>
#include <string>

namespace wfe {
	/// @brief A class implementing a debug message logger.
	class Logger {
	public:
		/// @brief An enum that represents the severity level of a message.
		enum MessageLevel {
			/// @brief Fatal error message severity. Used for errors the program cannot recover from.
			MESSAGE_LEVEL_FATAL = 1,
			/// @brief Error message severity. Used for errors which might alter the program's execution.
			MESSAGE_LEVEL_ERROR = 2,
			/// @brief Warning message severity. Used for slight bugs in the program, which shouldn't alter its execution.
			MESSAGE_LEVEL_WARNING = 4,
			/// @brief Info message severity. Used to display general info on the program's execuiton.
			MESSAGE_LEVEL_INFO = 8,
			/// @brief Debug message severity. Used to display messages useful for debugging.
			MESSAGE_LEVEL_DEBUG = 16,
			/// @brief Trace message severity. Used for messaging tracing function calls. Only necessary for debugging.
			MESSAGE_LEVEL_TRACE = 32,
			/// @brief A bitmask including all message severity levels, used in the logger's constructor to log all messages.
			MESSAGE_LEVEL_ALL = MESSAGE_LEVEL_FATAL | MESSAGE_LEVEL_ERROR | MESSAGE_LEVEL_WARNING | MESSAGE_LEVEL_INFO | MESSAGE_LEVEL_DEBUG | MESSAGE_LEVEL_TRACE,
			/// @brief A bitmask including all non-debug message severity levels, used in the logger's constructor to log all release message.
			MESSAGE_LEVEL_RELEASE = MESSAGE_LEVEL_FATAL | MESSAGE_LEVEL_ERROR | MESSAGE_LEVEL_WARNING | MESSAGE_LEVEL_INFO
		};
		/// @brief A mask type, used for specifying which message levels should be considered by the logger.
		typedef uint8_t MessageLevelMask;

		/// @brief Gets a string corresponding to the given message level. Used to output a message's severity level.
		/// @param level The severity level to convert.
		/// @return The string containing the level's name.
		static const char_t* GetMessageLevelString(MessageLevel level) {
			switch(level) {
			case MESSAGE_LEVEL_FATAL:
				return "[FATAL]  ";
			case MESSAGE_LEVEL_ERROR:
				return "[ERROR]  ";
			case MESSAGE_LEVEL_WARNING:
				return "[WARNING]";
			case MESSAGE_LEVEL_INFO:
				return "[INFO]   ";
			case MESSAGE_LEVEL_DEBUG:
				return "[DEBUG]  ";
			case MESSAGE_LEVEL_TRACE:
				return "[TRACE]  ";
			default:
				return "[MESSAGE]";
			}
		}

		/// @brief Creates a debug logger.
		/// @param filePath The math of the file the logger should output to.
		/// @param messageLevels A bitmask containing the message severity levels the logger shouldn't ignore. Defaulted to include all levels.
		/// @param logConsole True if the logger should output to the console, otherwise false. True by default.
		Logger(const char_t* filePath, MessageLevelMask messageLevels = MESSAGE_LEVEL_ALL, bool logConsole = true) : messageLevels(messageLevels), logConsole(logConsole) {
			// Open the output file stream
			outStream.open(filePath);
		}
		Logger(const Logger&) = delete;
		Logger(Logger&&) = delete;

		Logger& operator=(const Logger&) = delete;
		Logger& operator=(Logger&&) = delete;

		/// @brief Logs a message.
		/// @param level The message's severity level.
		/// @param message The message string.
		void LogMessage(MessageLevel level, const std::string& message) {
			// Exit the function if the message level should be ignored
			if(!(level & messageLevels))
				return;
			
			// Output the message
			const char_t* levelStr = GetMessageLevelString(level);
			outStream << levelStr << ' ' << message << '\n';
			if(logConsole)
				std::cout << levelStr << ' ' << message << '\n';
		}

		/// @brief Destroys the logger.
		~Logger();
	private:
		std::ofstream outStream;
		MessageLevelMask messageLevels;
		bool logConsole;
	};
}