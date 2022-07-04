#pragma once

#include "pair.hpp"
#include "IO.hpp"
#include <assert.h>

namespace wfe {
    template<class Value>
    class set {
    public:
        constexpr set() : _size(0), _capacity(0), _values(nullptr) {

        } 
        constexpr set(const set& other) : _size(other._size), _capacity(other._capacity), _values(new Value[_capacity]) {
            // Copy every value
            for(size_t i = 0; i < _size; ++i)
                _values[i] = other._values[i];
        }
        constexpr set(set&& other) noexcept : _size(other.size), _capacity(other._capacity), _values(other._values) {
            // Unlink the value list from the other set
            other._values = nullptr;
        }
        constexpr set(const Value* begin, const Value* end) : _size((size_t)(end - begin)), _capacity(1), _values(new Value[_size]) {
            // Find the lowest possible capacity
            while(_capacity < _size)
                _capacity <<= 1;
            
            // Create the array
            _values = new Value[_capacity];

            // Copy every value
            for(size_t i = 0; i < _size; ++i)
                _values[i] = begin[i];
        }
    
        set& operator=(const set& other) {
            // If the other map is the same as this one, exit the function
            if(&other == this)
                return *this;
            
            // Delete the old array
            delete[] _values;

            // Set the new values
            _size = other._size;
            _capacity = other._capacity;
            _values = new Value[_capacity];

            // Copy every value
            for(size_t i = 0; i < _size; ++i)
                _values[i] = other._values[i];
            
            return *this;
        }
        set& operator=(set&& other) noexcept {
            // Delete the old array
            delete[] _values;

            // Set the new values
            _size = other._size;
            _capacity = other._capacity;
            _values = other._values;

            // Unlink the value list from the other set
            other._values = nullptr;

            return *this;
        }
    
        Value& at(const Value& value) {
            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            // If the element's value is equal to the given value, return the element
            if(_values[pos] == value)
                return _values[pos];
            
            console::OutFatalError("Failed to find an element with the specified type!", 1);
            return *(Value*)0;
        }
        const Value& at(const Value& value) const {
            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            // If the element's value is equal to the given value, return the element
            if(_values[pos] == value)
                return _values[pos];
            
            console::OutFatalError("Failed to find an element with the specified type!", 1);
            return *(const Value*)0;
        }
    
        Value* find(const Value& value) {
            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            // If the element's value is equal to the given value, return the element
            if(_values[pos] == value)
                return _values + pos;
            
            return _values + _size;
        }
        const Value* find(const Value& value) const {
            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            // If the element's value is equal to the given value, return the element
            if(_values[pos] == value)
                return _values + pos;
            
            return _values + _size;
        }
    
        Value& operator[](const Value& value) {
            if(!_size) {
                // Create the array
                _size = 1;
                _capacity = 1;

                _values = new Value[1];
                *_values = value;

                return *_values;
            }

            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            // If the element's value is equal to the given value, return the element
            if(_values[pos] == value)
                return _values[pos];
            
            // Insert the element into the array
            if(_values[pos] < value)
                ++pos;
            ++_size;

            // Recreate the vector if needed
            if(_size > _capacity) {
                _capacity = 1;
                while(_capacity < _size)
                    _capacity <<= 1;
                
                Value* newValues = new Value[_capacity];

                for(size_t i = 0; i < pos; ++i)
                    newValues[i] = _values[i];
                newValues[pos] = value;
                for(size_t i = pos; i < _size - 1; ++i)
                    newValues[i + 1] = _values[i];

                delete[] _values;
                _values = newValues;
            } else {
                for(size_t i = _size - 1; i > pos; --i)
                    _values[i] = _values[i - 1];
                _values[pos] = value;
            }

            return _values[pos];
        }
        Value& operator[](Value&& value) {
            if(!_size) {
                // Create the array
                _size = 1;
                _capacity = 1;

                _values = new Value[1];
                *_values = value;

                return _values;
            }

            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            // If the element's value is equal to the given value, return the element
            if(_values[pos] == value)
                return _values[pos];
            
            // Insert the element into the array
            if(_values[pos] < value)
                ++pos;
            ++_size;

            // Recreate the vector if needed
            if(_size > _capacity) {
                _capacity = 1;
                while(_capacity < _size)
                    _capacity <<= 1;
                
                Value* newValues = new Value[_capacity];

                for(size_t i = 0; i < pos; ++i)
                    newValues[i] = _values[i];
                newValues[pos] = value;
                for(size_t i = pos; i < _size - 1; ++i)
                    newValues[i + 1] = _values[i];

                delete[] _values;
                _values = newValues;
            } else {
                for(size_t i = _size - 1; i > pos; --i)
                    _values[i] = _values[i - 1];
                _values[pos] = value;
            }

            return _values[pos];
        }
    
        pair<Value*, bool> insert(const Value& value) {
            if(!_size) {
                // Create the array
                _size = 1;
                _capacity = 1;

                _values = new Value[1];
                *_values = value;

                return { _values, true };
            }

            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            // If the element's value is equal to the given value, return the element
            if(_values[pos] == value)
                return { _values + pos, false };
            
            // Insert the element into the array
            if(_values[pos] < value)
                ++pos;
            ++_size;

            // Recreate the vector if needed
            if(_size > _capacity) {
                _capacity = 1;
                while(_capacity < _size)
                    _capacity <<= 1;
                
                Value* newValues = new Value[_capacity];

                for(size_t i = 0; i < pos; ++i)
                    newValues[i] = _values[i];
                newValues[pos] = value;
                for(size_t i = pos; i < _size - 1; ++i)
                    newValues[i + 1] = _values[i];

                delete[] _values;
                _values = newValues;
            } else {
                for(size_t i = _size - 1; i > pos; --i)
                    _values[i] = _values[i - 1];
                _values[pos] = value;
            }

            return { _values + pos, true };
        }
        pair<Value*, bool> insert(Value&& value) {
            if(!_size) {
                // Create the array
                _size = 1;
                _capacity = 1;

                _values = new Value[1];
                *_values = value;

                return { _values, true };
            }

            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            // If the element's value is equal to the given value, return the element
            if(_values[pos] == value)
                return { _values + pos, false };
            
            // Insert the element into the array
            if(_values[pos] < value)
                ++pos;
            ++_size;

            // Recreate the vector if needed
            if(_size > _capacity) {
                _capacity = 1;
                while(_capacity < _size)
                    _capacity <<= 1;
                
                Value* newValues = new Value[_capacity];

                for(size_t i = 0; i < pos; ++i)
                    newValues[i] = _values[i];
                newValues[pos] = value;
                for(size_t i = pos; i < _size - 1; ++i)
                    newValues[i + 1] = _values[i];

                delete[] _values;
                _values = newValues;
            } else {
                for(size_t i = _size - 1; i > pos; --i)
                    _values[i] = _values[i - 1];
                _values[pos] = value;
            }

            return { _values + pos, true };
        }

        pair<Value*, bool> insert_or_assign(const Value& value) {
            if(!_size) {
                // Create the array
                _size = 1;
                _capacity = 1;

                _values = new Value[1];
                *_values = value;

                return { _values, true };
            }

            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            // If the element's value is equal to the given value, return the element
            if(_values[pos] == value) {
                _values[pos] = value;
                return { _values + pos, false };
            }
            
            // Insert the element into the array
            if(_values[pos] < value)
                ++pos;
            ++_size;

            // Recreate the vector if needed
            if(_size > _capacity) {
                _capacity = 1;
                while(_capacity < _size)
                    _capacity <<= 1;
                
                Value* newValues = new Value[_capacity];

                for(size_t i = 0; i < pos; ++i)
                    newValues[i] = _values[i];
                newValues[pos] = value;
                for(size_t i = pos; i < _size - 1; ++i)
                    newValues[i + 1] = _values[i];

                delete[] _values;
                _values = newValues;
            } else {
                for(size_t i = _size - 1; i > pos; --i)
                    _values[i] = _values[i - 1];
                _values[pos] = value;
            }

            return { _values + pos, true };
        }
        pair<Value*, bool> insert_or_assign(Value&& value) {
            if(!_size) {
                // Create the array
                _size = 1;
                _capacity = 1;

                _values = new Value[1];
                *_values = value;

                return { _values, true };
            }

            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            // If the element's value is equal to the given value, return the element
            if(_values[pos] == value)
                return { _values + pos, false };
            
            // Insert the element into the array
            if(_values[pos] < value)
                ++pos;
            ++_size;

            // Recreate the vector if needed
            if(_size > _capacity) {
                _capacity = 1;
                while(_capacity < _size)
                    _capacity <<= 1;
                
                Value* newValues = new Value[_capacity];

                for(size_t i = 0; i < pos; ++i)
                    newValues[i] = _values[i];
                newValues[pos] = value;
                for(size_t i = pos; i < _size - 1; ++i)
                    newValues[i + 1] = _values[i];

                delete[] _values;
                _values = newValues;
            } else {
                for(size_t i = _size - 1; i > pos; --i)
                    _values[i] = _values[i - 1];
                _values[pos] = value;
            }

            return { _values + pos, true };
        }

        Value* erase(const Value* pos) {
            // Assert that the given position must be in range
            assert(pos >= _values && "The given position must be in range!");
            assert(pos < _values + _size && "The given position must be in range!");

            // Erase the element at the specified position
            --_size;

            size_t ind = pos - (size_t)_values;
            for(size_t i = ind; i < _size; ++i)
                _values[i] = _values[i + 1];
            
            return (Value*)pos;
        }
        size_t erase(const Value& value) {
            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            // If the element's value is equal to the given value, remove the element
            if(_values[pos] == value) {
                // Erase the element at the position
                --_size;

                for(size_t i = pos; i < _size; ++i)
                    _values[i] = _values[i + 1];

                return 1;
            }

            return 0;
        }

              Value* begin()       { 
            return _values; 
        }
              Value* end()         { 
            return _values + _size; 
        }
        const Value* begin() const { 
            return _values; 
        }
        const Value* end()   const { 
            return _values + _size; 
        }

        size_t size() const { 
            return _size; 
        }

        constexpr ~set() {
            // Delete the array
            delete[] _values;
        }
    private:
        size_t _size{};
        size_t _capacity{};
        Value* _values{};
    };
}