#pragma once
#include <stdint.h>
#include <type_traits>
#include <limits>

template<unsigned size>
struct getIndexTypeBySize
{
    static const bool LessOrEq8 = size < std::numeric_limits<uint8_t>::max();
    static const bool LessOrEq16 = size < std::numeric_limits<uint16_t>::max();

    typedef typename std::conditional<
        LessOrEq8,
        uint8_t,
        typename std::conditional<
            LessOrEq16,
            uint16_t,
            uint32_t
        >::type
    >::type type;
};
