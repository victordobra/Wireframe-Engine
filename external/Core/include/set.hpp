#pragma once

#include "vector.hpp"

namespace mge {
    template<class Value>
    class set {
    public:
        typedef Value value_type;

        set() = default;
        set(const set&) = default;
        set(set&&) noexcept = default;

        set& operator=(const set&) = default;
        set& operator=(set&&) noexcept = default;

        Value& operator[](const Value& value) {
            // If the set is empty, add the element right away
            if(!values.size()) {
                values.insert(values.begin(), value);
                return values[0];
            }

            //Binary search for it
            size_t step = 1;
            while(step < values.size())
                step <<= 1;
            step >>= 1;

            size_t pos = 0;
            while(step) {
                if(pos + step < values.size() && values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            if(values.size() && values[pos] == value)
                return values[pos];
            
            //There is no element with the specified key, create it
            ++pos;
            values.insert(values.begin() + pos, value);

            return values[pos];
        }
        Value& operator[](Value&& value) {
            // If the set is empty, add the element right away
            if(!values.size()) {
                values.insert(values.begin(), value);
                return values[0];
            }

            //Binary search for it
            size_t step = 1;
            while(step < values.size())
                step <<= 1;
            step >>= 1;

            size_t pos = 0;
            while(step) {
                if(pos + step < values.size() && values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            if(values.size() && values[pos] == value)
                return values[pos];
            
            //There is no element with the specified key, create it
            ++pos;
            values.insert(values.begin() + pos, value);

            return values[pos];
        }

        Value& insert(const value_type& value) {
            // If the set is empty, add the element right away
            if(!values.size()) {
                values.insert(values.begin(), value);
                return values[0];
            }
            
            //Binary search for it
            size_t step = 1;
            while(step < values.size())
                step <<= 1;
            step >>= 1;

            size_t pos = 0;
            while(step) {
                if(pos + step < values.size() && values[pos + step] <= value)
                    pos += step;
                step >>= 1;
            }

            if(values.size() && values[pos] == value)
                return values[pos];
            
            //There is no element with the specified key, create it
            ++pos;
            values.insert(values.begin() + pos, value);

            return values[pos];
        }
        Value& insert_or_assign(const value_type& value) {
            return insert(value);
        }
        
        value_type* begin() {
            return values.begin();
        }
        value_type* end() {
            return values.end();
        }
        const value_type* begin() const {
            return values.begin();
        }
        const value_type* end()   const {
            return values.end();
        }

        ~set() = default;
    private:
        vector<Value> values;
    };
}