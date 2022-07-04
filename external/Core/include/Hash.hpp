#pragma once

#include "Defines.hpp"
#include <typeinfo>

namespace wfe {
    template<class T>
    void GetHashCode(T val, size_t& hash);
    template<class T>
    void GetHashCode(T* val, size_t& hash) {
        hash = (size_t)val;
    }

    template<class T>
    void CombineHash(size_t& seed, const T& val) {
        size_t hash;
        GetHashCode(val, hash);
        seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    
    template<class T>
    size_t GetHashCode(T val) { 
        size_t hash = 0; 
        GetHashCode(val, hash); 
        return hash; 
    }
}