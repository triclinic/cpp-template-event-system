#pragma once

#include "buffer.h"
#include "statics.h"
#include <type_traits>

template<unsigned SIZE>
class StackIndexIterator
{
    static const typename getIndexTypeBySize<SIZE>::type _default = typename getIndexTypeBySize<SIZE>::type();
public:
    typedef typename getIndexTypeBySize<SIZE>::type index_t;

    inline void nextInput()
    {
        head++;
    }

    inline index_t getInputPosition() const
    {
        return head;
    }

    inline void nextOutput()
    {
        head--;
    }

    inline index_t getOutputPosition() const
    {
        return head - 1;
    }

    inline void reset()
    {
        head = _default;
    }

    inline index_t getIndexByCount(const index_t i) const
    {
        return i;
    }

    inline index_t size() const
    {
        return head;
    }

    inline bool isNotCeil() const
    {
        return head < SIZE;
    }

    inline bool isNotFloor() const
    {
        return head != _default;
    }
private:
    index_t head;
};

template<
        class T = unsigned char,
        unsigned SIZE = 32u>
using StackBuffer = Buffer<T, SIZE, StackIndexIterator<SIZE>>;
