#pragma once

#include "vector.hpp"
#include "pair.hpp"

namespace mge {
    template<class Key, class Value>
    class map {
    public:
        typedef pair<Key, Value> pair_type;

        map() = default;
        map(const map&) = default;
        map(map&&) noexcept = default;

        map& operator=(const map&) = default;
        map& operator=(map&&) noexcept = default;

        Value& operator[](const Key& key) {
            // Binary search for it
            size_t pos = 0, step = 1;

            while(step < pairs.size())
                step <<= 1;
            
            while(step) {
                if(pos + step < pairs.size() && pairs[pos + step].val1 <= key)
                    pos += step;
                step >>= 1;
            }

            if(pairs.size() && pairs[pos].val1 == key)
                return pairs[pos].val2;
            
            //There is no element with the specified key, create it
            pair_type pair;
            pair.val1 = key;

            pairs.insert(pairs.begin() + pos, pair);

            return pairs[pos].val2;
        }
        Value& operator[](Key&& key) {
            // Binary search for it
            size_t pos = 0, step = 1 << 31;
            
            while(step) {
                if(pos + step < pairs.size() && pairs[pos + step].val1 <= key)
                    pos += step;
                step >>= 1;
            }

            if(pairs.size() && pairs[pos].val1 == key)
                return pairs[pos].val2;
            
            //There is no element with the specified key, create it
            pair_type pair;
            pair.val1 = key;

            pairs.insert(pairs.begin() + pos, pair);

            return pairs[pos];
        }
        const Value& operator[](const Key& key) const {
            //Binary search for it
            size_t begin = 0, end = pairs.size();
            
            while(begin != end) {
                size_t mid = (begin + end) >> 1;

                if(pairs[mid].val1 > key) {
                    end = mid;
                } else {
                    begin = mid;
                }
            }

            if(pairs.size() && pairs[begin].val1 == key)
                return pairs[begin].val2;
            
            //There is no element with the specified key, return a null reference
            return *(Value*)0;
        }
        const Value& operator[](Key&& key)      const {
            //Binary search for it
            size_t begin = 0, end = pairs.size();
            
            while(begin != end) {
                size_t mid = (begin + end) >> 1;

                if(pairs[mid].val1 > key) {
                    end = mid;
                } else {
                    begin = mid;
                }
            }

            //There is no element with the specified key, return a null reference
            return *(Value*)0;
        }

        Value& insert(const Key& key, const Value& value) {
            return insert({key, value});
        }
        Value& insert(const pair_type& pair) {
            size_t begin = 0, end = 0;
            if(pairs.size()) {
                //Binary search for it
                end = pairs.size() - 1;
                
                while(begin != end) {
                    size_t mid = (begin + end) >> 1;

                    if(pairs[mid].val1 == pair.val1) {
                        end = mid;
                        begin = mid;
                    } else if(pairs[mid].val1 > pair.val1) {
                        end = mid - 1;
                    } else {
                        begin = mid + 1;
                    }
                }

                if(pairs[begin].val1 == pair.val1) {
                    pairs[begin].val2 = pair.val2;
                    return pairs[begin].val2;
                }
            }
            
            //There is no element with the specified key, create it
            pairs.insert(pairs.begin() + begin, pair);

            return pairs[begin].val2;
        }
        Value& insert_or_assign(const Key& key, const Value& value) {
            return insert({key, value});
        }
        Value& insert_or_assign(const pair_type& pair) {
            return insert(pair);
        }
        
        pair_type* begin() {
            return pairs.begin();
        }
        pair_type* end() {
            return pairs.end();
        }
        const pair_type* begin() const {
            return pairs.begin();
        }
        const pair_type* end()   const {
            return pairs.end();
        }

        ~map() = default;
    private:
        vector<pair_type> pairs;
    };
}