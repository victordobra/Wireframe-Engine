#pragma once
#include <typeinfo>
#include "string.hpp"
#include "BuildInfo.hpp"
#include "Defines.hpp"

namespace wfe {
    class TypeInfo {
    public:
        TypeInfo() = delete;
        TypeInfo(const TypeInfo&) = default;
        TypeInfo(TypeInfo&&) noexcept = default;
        TypeInfo(const std::type_info& other) : name(other.name()) {

        }

        TypeInfo& operator=(const TypeInfo&) = default;
        TypeInfo& operator=(TypeInfo&&) noexcept = default;

        /// @brief Returns the name of the type.
        string GetName() {
            return name;
        }
        /// @brief Hashes the type's name in an uint32_t.
        uint32_t Hash32() {
            uint32_t hash = 0x811c9dc5;
            const uint32_t prime = 0x1000193;

            for(size_t i = 0; i < name.length(); ++i) {
                uint8_t value = name[i];
                hash = hash ^ value;
                hash *= prime;
            }

            return hash;
        }
        /// @brief Hashes the type's name in an uint64_t.
        uint64_t Hash64() {
            uint64_t hash = 0x811c9dc5;
            const uint64_t prime = 0x1000193;

            for(size_t i = 0; i < name.length(); ++i) {
                uint8_t value = name[i];
                hash = hash ^ value;
                hash *= prime;
            }

            return hash;
        }
        /// @brief Hashes the type's name.
        size_t Hash() {
            size_t hash = 0x811c9dc5;
            const size_t prime = 0x1000193;

            for(size_t i = 0; i < name.length(); ++i) {
                uint8_t value = name[i];
                hash = hash ^ value;
                hash *= prime;
            }

            return hash;
        }


        ~TypeInfo() = default;
    private:
        string name{};
    };

    #define WFE_TYPE_ID(expr) wfe::TypeInfo(typeid(expr))
}