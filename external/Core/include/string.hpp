#pragma once

#include "Defines.hpp"
#include <string.h>

namespace wfe {
    class string {
    public:
        static constexpr size_t MAX_SIZE = 16777216; // 2 ^ 24

        constexpr string() : str(nullptr), _capacity(0) { 
        
        }
        string(const string& other) : _capacity(other._capacity), str(new char_t[other._capacity]) {
            strcpy(str, other.str);
        }
        string(string&& other) noexcept : _capacity(other._capacity), str(other.str) {
            //Unassign the other vector's string pointer
            other.str = nullptr;
        }
        string(const char_t* other) : _capacity(1), str(nullptr) {
            // Get the size of the string
            size_t strLength;

            strLength = strlen(other);
            ++strLength;

            // Find the lowest possible capacity
            while(_capacity < strLength) {
                _capacity = _capacity << 1;
            }

            str = new char_t[_capacity];
            memcpy(str, other, strLength);
        }

        string& operator=(const string& other);
        string& operator=(string&& other) noexcept;
        string& operator+=(const string& other);
        string& operator+=(string&& other) noexcept;

        string operator+(const string& other) const;
        string operator+(string&& other) const;

        bool8_t operator==(const string& other) const { return strcmp(str, other.c_str()) == 0; }
        bool8_t operator==(string&& other)      const { return strcmp(str, other.c_str()) == 0; }
        bool8_t operator< (const string& other) const { return strcmp(str, other.c_str()) <  0; }
        bool8_t operator< (string&& other)      const { return strcmp(str, other.c_str()) <  0; }
        bool8_t operator> (const string& other) const { return strcmp(str, other.c_str()) >  0; }
        bool8_t operator> (string&& other)      const { return strcmp(str, other.c_str()) >  0; }
        bool8_t operator<=(const string& other) const { return strcmp(str, other.c_str()) <= 0; }
        bool8_t operator<=(string&& other)      const { return strcmp(str, other.c_str()) <= 0; }
        bool8_t operator>=(const string& other) const { return strcmp(str, other.c_str()) >= 0; }
        bool8_t operator>=(string&& other)      const { return strcmp(str, other.c_str()) >= 0; }

        char_t& operator[](size_t ind)       { return str[ind]; }
        char_t  operator[](size_t ind) const { return str[ind]; }

        string& resize(size_t newSize);
        string& reserve(size_t newCapacity);
        string& append(const string& other);
        string& append(size_t n, char_t c);
        string& push_back(char_t c);
        string& pop_back();
        string& assign(const string& other);
        string& insert(size_t pos, const string& str);
        string& insert(size_t pos, size_t n, char_t c);
        string& erase(size_t pos, size_t len = -1);

        const char_t* c_str()    const { return str; }
        char_t*       data()           { return str; }
        const char_t* data()     const { return str; }
        size_t        size()     const { 
            return strnlen(str, _capacity);
        }
        size_t        length()   const { 
            return strnlen(str, _capacity);
        }
        size_t        max_size() const { return MAX_SIZE; }
        size_t        capacity() const { return _capacity; }

        constexpr ~string()  {
            delete[] str;
        }
    private:
        char_t* str{};
        size_t _capacity{};
    };

     int32_t  stoi  (const string& str, char_t** endPtr = nullptr, size_t base = 10);
     int32_t  stol  (const string& str, char_t** endPtr = nullptr, size_t base = 10);
     int64_t  stoll (const string& str, char_t** endPtr = nullptr, size_t base = 10);
    uint32_t  stoui (const string& str, char_t** endPtr = nullptr, size_t base = 10);
    uint32_t  stoul (const string& str, char_t** endPtr = nullptr, size_t base = 10);
    uint64_t  stoull(const string& str, char_t** endPtr = nullptr, size_t base = 10);
    float32_t stof  (const string& str, char_t** endPtr = nullptr);
    float64_t stod  (const string& str, char_t** endPtr = nullptr);

    template<class T>
    string ToString(T val);
}