#pragma once
#include "statics.h"
#include <stdint.h>

template<class T = unsigned char, unsigned SIZE = 32>
class CircularBuffer
{
    template<unsigned size>
    struct getIndexTypeBySize
    {
        static const bool LessOrEq8 = size <= 0xff;
        static const bool LessOrEq16 = size <= 0xffff;

        typedef typename static_if<
            LessOrEq8,
            uint8_t,
            typename static_if<
                LessOrEq16,
                uint16_t,
                uint32_t
            >::type
        >::type type;
    };

    template<unsigned size, typename IT>
    struct maskedIndexTraverser
    {
        static const IT mask = size - 1;
        static inline IT next(volatile const IT& idx)
        {
            return (idx + 1) & mask;
        }
    };

    template<unsigned size, typename IT>
    struct addsubIndexTraverser
    {
        static inline IT next(volatile const IT& idx)
        {
            return  (idx + 1u < size) ? (idx + 1u) : 0u;
        }
    };

    template<unsigned size>
    struct indexIterator
    {
        static const bool isPowerTwo = (size&(size-1))==0;

        typedef typename getIndexTypeBySize<size>::type index_t;
        static const index_t _default = index_t();
        typedef typename static_if<
            isPowerTwo,
            maskedIndexTraverser<size, index_t>,
            addsubIndexTraverser<size, index_t>
        >::type traverser_t;
    };

    typedef indexIterator<SIZE> iter_t;
    typedef typename iter_t::traverser_t Trv;

    volatile typename iter_t::index_t tail, head, count;
    T buffer[SIZE];

public:
    template<class TT, unsigned s>
    friend std::ostream& operator<<(std::ostream& os, const CircularBuffer<TT, s>& b);

    typedef typename iter_t::index_t index_t;
    CircularBuffer():tail{}, head{}, count{}{}

    inline void write(T&& value)
    {
        if(isNotFull()) {
            buffer[head] = std::move(value);
            count++;
            head = Trv::next(head);
        }
    }

    inline T read()
    {
        T value{};
        if(isNotEmpty()){
            value = std::move(buffer[tail]);
            count--;
            tail = Trv::next(tail);
        }
        return value;
    }

    inline void clear()
    {
        count = iter_t::_default;
        head = tail;
    }

    inline index_t capacity() const
    {
        return count;
    }
    // Flags
    inline bool isNotEmpty() const
    {
        return count != iter_t::_default;
    }

    inline bool isNotFull() const
    {
        return count < SIZE;
    }

    // Operators
    inline CircularBuffer& operator<<(T&& value)
    {
        write(std::move(value));
        return *this;
    }

    inline CircularBuffer& operator>>(T& value)
    {
        value = read();
        return *this;
    }
};

template<class T, unsigned s>
std::ostream& operator<<(std::ostream& os, const CircularBuffer<T, s>& b)
{
    os << "Circular buffer state: [";
    for(unsigned i = 0; i < s; i++){
        os << b.buffer[i] << ", ";
    }
    os << "]" << std::endl;
    os << "Circular buffer repr: [";
    typename CircularBuffer<T, s>::iter_t::index_t start = b.tail;
    for(; start != b.head; start = CircularBuffer<T, s>::Trv::next(start)){
        os << b.buffer[start] << ", ";
    }
    os << "]" << std::endl;
    os << "Count: " << (unsigned)b.count << std::endl;
    os << "Tail: " << (unsigned)b.tail << std::endl;
    os << "Head: " << (unsigned)b.head << std::endl;
    return os;
}
