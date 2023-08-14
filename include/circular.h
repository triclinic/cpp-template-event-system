#pragma once

#include "buffer.h"
#include "statics.h"
#include <type_traits>

template<unsigned SIZE>
class CircularIndexIterator
{
    template<unsigned size2, typename IT>
    struct maskedIndexTraverser
    {
        static const IT mask = size2 - 1;
        static inline IT next(volatile const IT& idx)
        {
            return (idx + 1) & mask;
        }
    };

    template<unsigned size2, typename IT>
    struct addsubIndexTraverser
    {
        static inline IT next(volatile const IT& idx)
        {
            return  (idx + 1u < size2) ? (idx + 1u) : 0u;
        }
    };
    static const bool isPowerTwo = (SIZE&(SIZE-1))==0;
    static const typename getIndexTypeBySize<SIZE>::type _default = typename getIndexTypeBySize<SIZE>::type();

    typedef typename std::conditional<
        isPowerTwo,
        maskedIndexTraverser<SIZE, typename getIndexTypeBySize<SIZE>::type>,
        addsubIndexTraverser<SIZE, typename getIndexTypeBySize<SIZE>::type>
    >::type traverser_t;

public:
    typedef typename getIndexTypeBySize<SIZE>::type index_t;

    inline void nextInput()
    {
        count++;
        head = traverser_t::next(head);
    }

    inline index_t getInputPosition() const
    {
        return head;
    }

    inline void nextOutput()
    {
        count--;
        tail = traverser_t::next(tail);
    }

    inline index_t getOutputPosition() const
    {
        return tail;
    }

    inline void reset()
    {
        count = _default;
        head = tail;
    }

    inline index_t getIndexByCount(const index_t i) const
    {
        if(i >= SIZE - tail){
            return i - (SIZE - tail);
        }
        return tail + i;
    }

    inline index_t size() const
    {
        return count;
    }

    inline bool isNotCeil() const
    {
        return count < SIZE;
    }

    inline bool isNotFloor() const
    {
        return count != _default;
    }
private:
    index_t tail, head, count;
};

template<
        class T = unsigned char,
        unsigned SIZE = 32u>
using CircularBuffer = Buffer<T, SIZE, CircularIndexIterator<SIZE>>;
