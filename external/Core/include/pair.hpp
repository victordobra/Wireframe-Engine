#pragma once

namespace wfe {
    template<class T1, class T2>
    class pair {
    public:
        T1 val1{};
        T2 val2{};

        constexpr pair() = default;
        constexpr pair(const pair&) = default;
        constexpr pair(pair&&) noexcept = default;
        constexpr pair(T1 v1, T2 v2) : val1(v1), val2(v2) { }

        pair& operator=(const pair&) = default;
        pair& operator=(pair&&) noexcept = default;

        constexpr ~pair() = default;
    };
}