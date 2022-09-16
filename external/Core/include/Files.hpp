#pragma once

#include "Defines.hpp"
#include "string.hpp"
#include <stdio.h>

namespace wfe {
    typedef enum : uint8_t {
        STREAM_TYPE_NONE       = 0,
        STREAM_TYPE_BINARY     = 1,
        STREAM_TYPE_AT_THE_END = 2,
        STREAM_TYPE_APPEND     = 4,
        STREAM_TYPE_TRUNCATE   = 8
    } StreamType;
    typedef enum : uint8_t {
        SEEK_RELATIVE_BEGINNING = SEEK_SET,
        SEEK_RELATIVE_POSITION = SEEK_CUR,
        SEEK_RELATIVE_END = SEEK_END
    } SeekRelative;

    class FileInput {
    public:
        FileInput();
        FileInput(const FileInput& other) = delete;
        FileInput(FileInput&& other) noexcept = delete;
        FileInput(const string& fileLocation, StreamType type = StreamType::STREAM_TYPE_NONE);

        FileInput& operator=(const FileInput& other) = delete;
        FileInput& operator=(FileInput&& other) noexcept = delete;

        bool8_t operator!();
        operator bool8_t();

        FileInput& Open(const string& fileLocation, StreamType type = StreamType::STREAM_TYPE_NONE);
        FileInput& Close();

        FileInput& Get();
        FileInput& ReadBuffer(char_t& c);
        FileInput& ReadBuffer(char_t* buffer, size_t count);

        FileInput& ReadLine(string& str, size_t size, char_t sep = '\n');

        FileInput& Read( int8_t  & num, const char_t* sep = " \n");
        FileInput& Read( int16_t & num, const char_t* sep = " \n");
        FileInput& Read( int32_t & num, const char_t* sep = " \n");
        FileInput& Read( int64_t & num, const char_t* sep = " \n");
        FileInput& Read(uint8_t  & num, const char_t* sep = " \n");
        FileInput& Read(uint16_t & num, const char_t* sep = " \n");
        FileInput& Read(uint32_t & num, const char_t* sep = " \n");
        FileInput& Read(uint64_t & num, const char_t* sep = " \n");
        FileInput& Read(float32_t& num, const char_t* sep = " \n");
        FileInput& Read(float64_t& num, const char_t* sep = " \n");
        FileInput& Read(string   & str, const char_t* sep = " \n");

        FileInput& Seek(ptrdiff_t pos, SeekRelative relative = SEEK_RELATIVE_BEGINNING);

        bool8_t IsOpen() const;
        bool8_t IsAtTheEnd() const;
        bool8_t IsBad() const;
        size_t Tell() const;
        size_t GetCount() const;
        char_t Peek() const;

        ~FileInput();
    private:
        FILE* file;
    };

    class FileOutput {
    public:
        FileOutput();
        FileOutput(const FileOutput&) = delete;
        FileOutput(FileOutput&&) noexcept = delete;
        FileOutput(const string& fileLocation, StreamType type = STREAM_TYPE_NONE);

        bool8_t operator!();
        operator bool8_t();

        FileOutput& Open(const string& fileLocation, StreamType type = STREAM_TYPE_NONE);
        FileOutput& Close();

        FileOutput& WriteBuffer(char_t c);
        FileOutput& WriteBuffer(char_t* buf, size_t size);
        
        template<class T>
        FileOutput& Write(const T& value) {
            return Write(ToString(value));
        }
        FileOutput& Write(char_t c);
        FileOutput& Write(const string& str);

        FileOutput& Flush();

        bool8_t IsOpen() const;
        bool8_t IsAtTheEnd() const;
        bool8_t IsBad() const;
        size_t Tell() const;
        size_t GetCount() const;
        char_t Peek() const;

        ~FileOutput();
    private:
        FILE* file;
    };
}