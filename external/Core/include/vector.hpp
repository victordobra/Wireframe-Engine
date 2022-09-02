#pragma once

#include "Defines.hpp"
#include <initializer_list>
#include <assert.h>

namespace wfe {
    template<class T>
    class vector {
    public:
        typedef T                 value_type;
        typedef size_t            size_type;
        typedef ptrdiff_t         difference_type;
        typedef value_type&       reference;
        typedef const value_type& const_reference;
        typedef value_type*       pointer;
        typedef const value_type* const_pointer;

        static constexpr size_type MAX_SIZE = 16777216; //2 ^ 24

        constexpr vector() : _size(0), _capacity(0), _data(nullptr) { 

        }
        constexpr vector(const vector& other) : _size(other._size), _capacity(other._capacity), _data(new value_type[_capacity]) {
            // Copy everything from the other vector
            for(size_type i = 0; i < _size; ++i)
                _data[i] = other._data[i];
        }
        constexpr vector(vector&& other) noexcept : _size(other._size), _capacity(other._capacity), _data(other._data) {
            // Unassign the other vector's data pointer
            other._data = nullptr;
        }
        constexpr vector(size_type _size) : _size(_size), _capacity(1) { 
            // Find the lowest possible capacity
            while(_capacity < _size) {
                _capacity = _capacity << 1;
            }
            // Create the data
            _data = new value_type[_capacity];
        }
        constexpr vector(std::initializer_list<value_type> _list) : _size(_list.size()), _capacity(1) {
            // Find the lowest possible capacity
            while(_capacity < _size) {
                _capacity = _capacity << 1;
            }
            // Create the data
            _data = new value_type[_capacity];

            // Copy everything from the initializer list
            const_pointer p = _list.begin();
            for(size_type i = 0; i < _size; ++i)
                _data[i] = *p++;
        }
        constexpr vector(const_pointer begin, const_pointer end) : _size((size_type)(end - begin)), _capacity(1) {
            // Find the lowest possible capacity
            while(_capacity < _size) {
                _capacity = _capacity << 1;
            }
            // Create the data
            _data = new value_type[_capacity];

            // Copy everything
            const_pointer p = begin;
            for(size_type i = 0; i < _size; ++i)
                _data[i] = *p++;
        }

        vector& operator=(const vector& other)  {
            // If the other vector is the same as this vector, exit the function
            if(&other == this)
                return *this;

            // Delete the old data
            delete[] _data;

            // Create the new data
            _size = other._size;
            _capacity = other._capacity;
            _data = new value_type[_capacity];

            // Copy all of the data over
            for(size_type i = 0; i < _size; ++i)
                _data[i] = other._data[i];

            return *this;
        }
        vector& operator=(vector&& other) noexcept {
            // Delete the old data
            delete[] _data;

            // Assign the other vector's data to this vector
            _data = other._data;
            _capacity = other._capacity;
            _size = other._size;

            // Unassign the other vector's data pointer
            other._data = nullptr;

            return *this;
        }

        reference operator[](size_type ind) {
            assert((ind < _size) && "Index is out of the vector's range!");

            return _data[ind];
        }
        const_reference operator[](size_type ind) const {
            assert((ind < _size) && "Index is out of the vector's range!");

            return _data[ind];
        }

        reference at(size_type ind) {
            assert((ind < _size) && "Index is out of the vector's range!");

            return _data[ind];
        }
        const_reference at(size_type ind) const {
            assert((ind < _size) && "Index is out of the vector's range!");

            return _data[ind];
        }

        vector& resize(size_type newSize) {
            // If the new size is equal to the current size, exit the function
            if (newSize == _size)
                return *this;
            
            // Assert that the new size must be lower than or equal to MAX_SIZE
            assert((newSize <= MAX_SIZE) && "A vector's size must always be lower than or equal to MAX_SIZE!");

            // Set the new size
            size_t minSize = (_size < newSize) ? _size : newSize;
            _size = newSize;

            // If the new size is still fit for the capacity, exit the function
            if(_size <= _capacity)
                return *this;

            // Progress to the next capacity
            _capacity = 1;
            while(_capacity < _size) {
                _capacity = _capacity << 1;
            }
            
            // Create the new data
            pointer newData = new value_type[_capacity];
            for(size_type i = 0; i < minSize; ++i)
                newData[i] = _data[i];
            
            // Delete the old data and replace it with the new one
            delete[] _data;
            _data = newData;
            
            return *this;
        }
        vector& reserve(size_type newCapacity) {
            // If the new capacity is equal to the current capacity, exit the function
            if (newCapacity == _capacity)
                return *this;
            
            // Assert that the new capacity must be lower than or equal to MAX_SIZE
            assert((newCapacity <= MAX_SIZE) && "A vector's capacity must always be lower than or equal to MAX_SIZE!");
            // Assert that the new capacity must be higher than or equal to the size of the vector
            assert((newCapacity < _size) && "A vector's capacity must be higher than or equal to the size of the vector!");

            // Set the new capacity
            _capacity = newCapacity;

            // Create the new data
            pointer newData = new value_type[_capacity];
            for(size_type i = 0; i < _size; ++i)
                newData[i] = _data[i];
            
            // Delete the old data and replace it with the new one
            delete[] _data;
            _data = newData;

            return *this;
        }
        vector& shrink_to_fit() {
            // If the capacity of the vector is equal to its size, exit the function
            if (_capacity == _size)
                return *this;

            // Set the capacity
            _capacity = _size;

            // Create the new data
            pointer newData = new value_type[_capacity];
            for(size_type i = 0; i < _size; ++i)
                newData[i] = _data[i];
            
            // Delete the old data and replace it with the new one
            delete[] _data;
            _data = newData;

            return *this;
        }
        vector& push_back(const_reference newElem) {
            resize(_size + 1);
            back() = newElem;
            return *this;
        }
        vector& pop_back() {
            if(_size)
                resize(_size - 1);
            return *this;
        }
        vector& insert(const_pointer position, const_reference newElem) {
            // Assert that the position is inside of the vector
            assert((position >= _data) && "The position must be inside of the vector!");
            assert((position <= _data + _size) && "The position must be inside of the vector!");

            // Find the index of the position
            size_type pos_ind = position - _data;

            // Resize the vector
            resize(_size + 1);

            // Move over all the evelemts to the right of the new element
            for(size_type i = _size - 1; i > pos_ind; i--)
                _data[i] = _data[i - 1];
            _data[pos_ind] = newElem;
            
            return *this;
        }
        vector& insert(const_pointer position, size_type num, const_reference newElem) {
            // Assert that the position is inside of the vector
            assert((position >= _data) && "The position must be inside of the vector!");
            assert((position <= _data + _size) && "The position must be inside of the vector!");

            // Find the index of the position
            size_type pos_ind = position - _data;

            // Resize the vector
            resize(_size + num);

            // Move over all the evelemts to the right of the new element
            for(size_type i = _size - 1; i >= pos_ind + num; i--)
                _data[i] = _data[i - num];

            pointer p = (pointer)position;
            for(size_type i = 0; i < num; i++)
                *p++ = newElem;
            
            return *this;
        }
        vector& insert(const_pointer position, const_pointer first, const_pointer last) {
            // Assert that the position is inside of the vector
            assert((position >= _data) && "The position must be inside of the vector!");
            assert((position <= _data + _size) && "The position must be inside of the vector!");
            // Assert that the last pointer must be bigger than or equal to the first
            assert((last >= first) && "The last pointer must be bigger than or equal to the first!");

            // Find the number of elements need to add
            size_type num = last - first;
            size_type pos_ind = position - _data;

            // Resize the vector
            resize(_size + num);

            // Move over all the evelemts to the right of the new element
            for(size_type i = _size - 1; i >= pos_ind + num; i--)
                _data[i] = _data[i - num];

            pointer p = (pointer)position;
            for(pointer elemP = (pointer)first; elemP < last; elemP++)
                *p++ = *elemP;
            
            return *this;
        }
        vector& erase(const_pointer position) {
            // Assert that the position is inside of the vector
            assert((position >= _data) && "The position must be inside of the vector!");
            assert((position <= _data + _size) && "The position must be inside of the vector!");

            // Resize the vector
            resize(_size - 1);

            // Move over all the evelemts to the right of the new element
            size_type pos_ind = position - _data;

            for(size_type i = pos_ind; i < _size; i++)
                _data[i] = _data[i + 1];
            
            return *this;
        }
        vector& erase(const_pointer first, const_pointer last) {
            // Assert that the positions are inside of the vector
            assert((first >= _data) && "The position must be inside of the vector!");
            assert((first <= _data + _size) && "The position must be inside of the vector!");

            assert((last >= _data) && "The position must be inside of the vector!");
            assert((last <= _data + _size) && "The position must be inside of the vector!");

            // Assert that the last pointer must be bigger than or equal to the first
            assert((last >= first) && "The last pointer must be bigger than or equal to the first!");

            // Find the number of elements need to remove
            size_type num = last - first + 1;

            // Resize the vector
            resize(_size - num);

            // Move over all the evelemts to the right of the new element
            size_type pos_ind = first - _data;

            for(size_type i = pos_ind; i < _size; i++)
                _data[i] = _data[i + num];

            return *this;
        }
        void swap(vector<T>& other) {
            _data += other._data;
            other._data = _data - other._data;
            _data -= other._data;

            _size += other._size;
            other._size = _size - other._size;
            _size -= other._size;
        }
        vector& clear() {
            resize(0);
            return *this;
        }

        reference       front()       { return *_data; }
        reference       back()        { return *(_data + _size - 1); }
        const_reference front() const { return *_data; }
        const_reference back()  const { return *(_data + _size - 1); }

        pointer       begin()       { return _data; }
        pointer       end()         { return _data + _size; }
        const_pointer begin() const { return _data; }
        const_pointer end()   const { return _data + _size; }

        size_type     size()     const { return _size;     } 
        size_type     capacity() const { return _capacity; } 
        size_type     max_size() const { return MAX_SIZE;  } 
        bool8_t       empty()    const { return !_size;    }
        pointer       data()           { return _data;     }
        const_pointer data()     const { return _data;     }

        constexpr ~vector() {
            delete[] _data;
        }
    private:
        size_type _size, _capacity;
        pointer _data;
    };
}