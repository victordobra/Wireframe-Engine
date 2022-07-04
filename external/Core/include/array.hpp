#pragma once

#include "Defines.hpp"
#include <assert.h>
#include <initializer_list>

namespace wfe {
    template<class T, size_t _size>
    class array {
    public:
        typedef T                 value_type;
        typedef value_type&       reference;
        typedef const value_type& const_reference;
        typedef value_type*       pointer;
        typedef const value_type* const_pointer;

        constexpr array() = default;
        constexpr array(const array&) = default;
        constexpr array(array&&) noexcept = default;
        
        array(std::initializer_list<value_type> list) {
            size_t minSize = (list.size() < _size) ? list.size() : _size;
            const_pointer p = list.begin();
            for(size_t i = 0; i < _size; i++)
                this->arr[i] = *p++;
        }

        array& operator=(const array&) = default;
        array& operator=(array&&) noexcept = default;

        reference operator[](size_t ind) { return arr[ind]; }
        const_reference operator[](size_t ind) const { return arr[ind]; }

        reference at(size_t ind) {
            assert((ind < _size) && "Index is out of the array's range!");

            return arr[ind];
        }
        const_reference at(size_t ind) const {
            assert((ind < _size) && "Index is out of the array's range!");

            return arr[ind];
        }

        reference       front()       { return arr[0]; }
        const_reference front() const { return arr[0]; }
        reference       back()        { return arr[_size - 1]; }
        const_reference back()  const { return arr[_size - 1]; }

        reference       start()       { return arr; }
        const_reference start() const { return arr; }
        reference       end()         { return arr + _size; }
        const_reference end()   const { return arr + _size; }

        bool8_t empty()    const { return !_size; }
        size_t  size()     const { return _size; }
        size_t  max_size() const { return _size; }
        pointer data()           { return arr; }

        array& fill(const_reference val) {
            for (size_t i = 0; i < _size; i++)
                arr[i] = val;
            return *this;
        }
        void swap(array<value_type, _size>& other) {
            value_type aux;

            for(size_t i = 0; i < _size; i++) {
                aux = other[i];
                other[i] = arr[i];
                arr[i] = aux;
            }
        }

        constexpr ~array() = default;
    private:
        value_type arr[_size]{};
    };
}