#pragma once

#include "pair.hpp"
#include "IO.hpp"
#include <assert.h>

namespace wfe {
    template<class Key, class Value>
    class map {
    public:
        typedef pair<Key, Value> pair_type;

        constexpr map() : _size(0), _capacity(0), _pairs(nullptr) {

        }
        constexpr map(const map& other) : _size(other._size), _capacity(other._capacity), _pairs(new pair_type[_capacity]) {
            // Copy every pair
            for(size_t i = 0; i < _size; ++i)
                _pairs[i] == other._pairs[i];
        }
        constexpr map(map&& other) noexcept : _size(other._size), _capacity(other._capacity), _pairs(other._pairs) {
            // Unkink the pair list from the other map
            other._pairs = nullptr;
        }
        constexpr map(const pair_type* begin, const pair_type* end) : _size((size_t)(end - begin)), _capacity(1), _pairs(nullptr) {
            // Find the lowest possible capacity
            while(_capacity < _size)
                _capacity <<= 1;
            
            // Create the array
            _pairs = new pair_type[_capacity];

            // Copy every pair
            for(size_t i = 0; i < _size; ++i)
                _pairs[i] = begin[i];
        }

        map& operator=(const map& other) {
            // If the other map is the same as this one, exit the function
            if(&other == this)
                return *this;
            
            // Delete the old list
            delete[] _pairs;

            // Set the new values
            _size = other._size;
            _capacity = other._capacity;
            _pairs = new pair_type[_capacity];

            // Copy every pair
            for(size_t i = 0; i < _size; ++i)
                _pairs[i] == other._pairs[i];
            
            return *this;
        }
        map& operator=(map&& other) noexcept {
            // Delete the old list
            delete[] _pairs;

            // Set the new values
            _size = other._size;
            _capacity = other._capacity;
            _pairs = other._pairs;

            // Unkink the pair list from the other map
            other._pairs = nullptr;

            return *this;
        }

        Value& at(const Key& key) {
            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _pairs[pos + step].val1 <= key)
                    pos += step;
                step >>= 1;
            }

            // If the element's key is equal to the given key, return the element
            if(_pairs[pos].val1 == key)
                return _pairs[pos].val2;
            
            console::OutFatalError("Failed to find an element with the specified type!", 1);
            return *(Value*)0;
        }
        const Value& at(const Key& key) const {
            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _pairs[pos + step].val1 <= key)
                    pos += step;
                step >>= 1;
            }

            // If the element's key is equal to the given key, return the element
            if(_pairs[pos].val1 == key)
                return _pairs[pos].val2;

            console::OutFatalError("Failed to find an element with the specified type!", 1);
            return *(const Value*)0;
        }

        pair_type* find(const Key& key) {
            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _pairs[pos + step].val1 <= key)
                    pos += step;
                step >>= 1;
            }

            // If the element's key is equal to the given key, return the element
            if(_pairs[pos].val1 == key)
                return _pairs + pos;
            
            return _pairs + _size;
        }
        const pair_type* find(const Key& key) const {
            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _pairs[pos + step].val1 <= key)
                    pos += step;
                step >>= 1;
            }

            // If the element's key is equal to the given key, return the element
            if(_pairs[pos].val1 == key)
                return _pairs + pos;
            
            return _pairs + _size;
        }

        Value& operator[](const Key& key) {
            if(!_size) {
                // Create the array
                _size = 1;
                _capacity = 1;

                _pairs = new pair_type[1];
                *_pairs = { key, {} };

                return _pairs->val2;
            }

            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _pairs[pos + step].val1 <= key)
                    pos += step;
                step >>= 1;
            }

            // If the element's key is equal to the given key, return the element
            if(_pairs[pos].val1 == key)
                return _pairs[pos].val2;
            
            // Insert the element into the array
            if(_pairs[pos].val1 < key)
                ++pos;
            ++_size;

            // Recreate the vector if needed
            if(_size > _capacity) {
                _capacity = 1;
                while(_capacity < _size)
                    _capacity <<= 1;
                
                pair_type* newPairs = new pair_type[_capacity];

                for(size_t i = 0; i < pos; ++i)
                    newPairs[i] = _pairs[i];
                newPairs[pos] = { key, {} };
                for(size_t i = pos; i < _size - 1; ++i)
                    newPairs[i + 1] = _pairs[i];

                delete[] _pairs;
                _pairs = newPairs;
            } else {
                for(size_t i = _size - 1; i > pos; --i)
                    _pairs[i] = _pairs[i - 1];
                _pairs[pos] = { key, {} };
            }

            return _pairs[pos].val2;
        }
        Value& operator[](Key&& key) {
            if(!_size) {
                // Create the array
                _size = 1;
                _capacity = 1;

                _pairs = new pair_type[1];
                *_pairs = { key, {} };

                return _pairs->val2;
            }

            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _pairs[pos + step].val1 <= key)
                    pos += step;
                step >>= 1;
            }

            // If the element's key is equal to the given key, return the element
            if(_pairs[pos].val1 == key)
                return _pairs[pos].val2;
            
            // Insert the element into the array
            if(_pairs[pos].val1 < key)
                ++pos;
            ++_size;

            // Recreate the vector if needed
            if(_size > _capacity) {
                _capacity = 1;
                while(_capacity < _size)
                    _capacity <<= 1;
                
                pair_type* newPairs = new pair_type[_capacity];

                for(size_t i = 0; i < pos; ++i)
                    newPairs[i] = _pairs[i];
                newPairs[pos] = { key, {} };
                for(size_t i = pos; i < _size - 1; ++i)
                    newPairs[i + 1] = _pairs[i];

                delete[] _pairs;
                _pairs = newPairs;
            } else {
                for(size_t i = _size - 1; i > pos; --i)
                    _pairs[i] = _pairs[i - 1];
                _pairs[pos] = { key, {} };
            }

            return _pairs[pos].val2;
        }

        pair<pair_type*, bool> insert(const pair_type& pair) {
            if(!_size) {
                // Create the array
                _size = 1;
                _capacity = 1;

                _pairs = new pair_type[1];
                *_pairs = pair;

                return { _pairs, true };
            }

            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _pairs[pos + step].val1 <= pair.val1)
                    pos += step;
                step >>= 1;
            }

            // If the element's key is equal to the given key, return the element
            if(_pairs[pos].val1 == pair.val1)
                return { _pairs + pos, false };
            
            // Insert the element into the array
            if(_pairs[pos].val1 < pair.val1)
                ++pos;
            ++_size;

            // Recreate the vector if needed
            if(_size > _capacity) {
                _capacity = 1;
                while(_capacity < _size)
                    _capacity <<= 1;
                
                pair_type* newPairs = new pair_type[_capacity];

                for(size_t i = 0; i < pos; ++i)
                    newPairs[i] = _pairs[i];
                newPairs[pos] = pair;
                for(size_t i = pos; i < _size - 1; ++i)
                    newPairs[i + 1] = _pairs[i];

                delete[] _pairs;
                _pairs = newPairs;
            } else {
                for(size_t i = _size - 1; i > pos; --i)
                    _pairs[i] = _pairs[i - 1];
                _pairs[pos] = pair;
            }

            return { _pairs + pos, true };
        }
        pair<pair_type*, bool> insert(pair_type&& pair) {
            if(!_size) {
                // Create the array
                _size = 1;
                _capacity = 1;

                _pairs = new pair_type[1];
                *_pairs = pair;

                return { _pairs, true };
            }

            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _pairs[pos + step].val1 <= pair.val1)
                    pos += step;
                step >>= 1;
            }

            // If the element's key is equal to the given key, return the element
            if(_pairs[pos].val1 == pair.val1)
                return { _pairs + pos, false };
            
            // Insert the element into the array
           if(_pairs[pos].val1 < pair.val1)
                ++pos;
            ++_size;

            // Recreate the vector if needed
            if(_size > _capacity) {
                _capacity = 1;
                while(_capacity < _size)
                    _capacity <<= 1;
                
                pair_type* newPairs = new pair_type[_capacity];

                for(size_t i = 0; i < pos; ++i)
                    newPairs[i] = _pairs[i];
                newPairs[pos] = pair;
                for(size_t i = pos; i < _size - 1; ++i)
                    newPairs[i + 1] = _pairs[i];

                delete[] _pairs;
                _pairs = newPairs;
            } else {
                for(size_t i = _size - 1; i > pos; --i)
                    _pairs[i] = _pairs[i - 1];
                _pairs[pos] = pair;
            }

            return { _pairs + pos, true };
        }

        pair<pair_type*, bool> insert_or_assign(const pair_type& pair) {
            if(!_size) {
                // Create the array
                _size = 1;
                _capacity = 1;

                _pairs = new pair_type[1];
                *_pairs = pair;

                return { _pairs, true };
            }

            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _pairs[pos + step].val1 <= pair.val1)
                    pos += step;
                step >>= 1;
            }

            // If the element's key is equal to the given key, return the element
            if(_pairs[pos].val1 == pair.val1) {
                _pairs[pos].val2 = pair.val2;
                return { _pairs + pos, false };
            }
            
            // Insert the element into the array
            if(_pairs[pos].val1 < pair.val1)
                ++pos;
            ++_size;

            // Recreate the vector if needed
            if(_size > _capacity) {
                _capacity = 1;
                while(_capacity < _size)
                    _capacity <<= 1;
                
                pair_type* newPairs = new pair_type[_capacity];

                for(size_t i = 0; i < pos; ++i)
                    newPairs[i] = _pairs[i];
                newPairs[pos] = { pair.val1, {} };
                for(size_t i = pos; i < _size - 1; ++i)
                    newPairs[i + 1] = _pairs[i];

                delete[] _pairs;
                _pairs = newPairs;
            } else {
                for(size_t i = _size - 1; i > pos; --i)
                    _pairs[i] = _pairs[i - 1];
                _pairs[pos] = { pair.val1, {} };
            }

            return { _pairs + pos, true };
        }
        pair<pair_type*, bool> insert_or_assign(pair_type&& pair) {
            if(!_size) {
                // Create the array
                _size = 1;
                _capacity = 1;

                _pairs = new pair_type[1];
                *_pairs = pair;

                return { _pairs, true };
            }

            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _pairs[pos + step].val1 <= pair.val1)
                    pos += step;
                step >>= 1;
            }

            // If the element's key is equal to the given key, return the element
            if(_pairs[pos].val1 == pair.val1) {
                _pairs[pos].val2 = pair.val2;
                return { _pairs + pos, false };
            }
            
            // Insert the element into the array
            if(_pairs[pos].val1 < pair.val1)
                ++pos;
            ++_size;

            // Recreate the vector if needed
            if(_size > _capacity) {
                _capacity = 1;
                while(_capacity < _size)
                    _capacity <<= 1;
                
                pair_type* newPairs = new pair_type[_capacity];

                for(size_t i = 0; i < pos; ++i)
                    newPairs[i] = _pairs[i];
                newPairs[pos] = { pair.val1, {} };
                for(size_t i = pos; i < _size - 1; ++i)
                    newPairs[i + 1] = _pairs[i];

                delete[] _pairs;
                _pairs = newPairs;
            } else {
                for(size_t i = _size - 1; i > pos; --i)
                    _pairs[i] = _pairs[i - 1];
                _pairs[pos] = { pair.val1, {} };
            }

            return { _pairs + pos, true };
        }

        pair_type* erase(const pair_type* pos) {
            // Assert that the given position must be in range
            assert(pos >= _pairs && "The given position must be in range!");
            assert(pos < _pairs + _size && "The given position must be in range!");

            // Erase the element at the specified position
            --_size;

            size_t ind = pos - (size_t)_pairs;
            for(size_t i = ind; i < _size - 1; ++i)
                _pairs[i] = _pairs[i + 1];
            
            return (pair_type*)pos;
        }
        size_t erase(const Key& key) {
            // Binary search for the element
            size_t pos = 0, step = 1;

            while(step < _size)
                step <<= 1;
            step >>= 1;

            while(step) {
                if(pos + step < _size && _pairs[pos + step].val1 <= key)
                    pos += step;
                step >>= 1;
            }

            // If the element's key is equal to the given key, remove the element
            if(_pairs[pos].val1 == key) {
                // Erase the element at the position
                --_size;

                for(size_t i = pos; i < _size - 1; ++i)
                    _pairs[i] = _pairs[i + 1];

                return 1;
            }

            return 0;
        }

              pair_type* begin()       { 
            return _pairs; 
        }
              pair_type* end()         { 
            return _pairs + _size; 
        }
        const pair_type* begin() const { 
            return _pairs; 
        }
        const pair_type* end()   const { 
            return _pairs + _size; 
        }

        size_t size() const { 
            return _size; 
        }

        constexpr ~map() {
            // Delete the pairs
            delete[] _pairs;
        }
    private:
        size_t _size{};
        size_t _capacity{};
        pair_type* _pairs{};
    };
}