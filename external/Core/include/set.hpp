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
            //Binary search for it
            size_t begin = 0, end = values.size();
            
            while(begin != end) {
                size_t mid = (begin + end) >> 1;

                if(values[mid].val1 > value) {
                    end = mid;
                } else {
                    begin = mid;
                }
            }

            if(values.size() && values[begin] == value)
                return values[begin];
            
            //There is no element with the specified key, create it
            values.insert(values.begin() + begin, value);

            return values[begin];
        }
        Value& operator[](Value&& value) {
            //Binary search for it
            size_t begin = 0, end = values.size();
            
            while(begin != end) {
                size_t mid = (begin + end) >> 1;

                if(values[mid].val1 > value) {
                    end = mid;
                } else {
                    begin = mid;
                }
            }

            if(values.size() && values[begin] == value)
                return values[begin];
            
            //There is no element with the specified key, create it
            values.insert(values.begin() + begin, value);

            return values[begin];
        }

        Value& insert(const value_type& value) {
            size_t begin = 0, end = 0;
            if(values.size()) {
                //Binary search for it
                end = values.size() - 1;
                
                while(begin != end) {
                    size_t mid = (begin + end) >> 1;

                    if(values[mid] == value) {
                        end = mid;
                        begin = mid;
                    } else if(values[mid] > value) {
                        end = mid - 1;
                    } else {
                        begin = mid + 1;
                    }
                }

                if(values[begin] == value) {
                    values[begin] = value;
                    return values[begin];
                }
            }
            
            //There is no element with the specified key, create it
            values.insert(values.begin() + begin, value);

            return values[begin];
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