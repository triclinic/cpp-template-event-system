#pragma once

#include <utility>
#include <type_traits>

template<
        class T,
        unsigned SIZE,
        class IT>
class Buffer
{
public:
    typedef typename IT::index_t index_t;
    //typedef typename T value_t;

//    template<class TT, unsigned s, class ITT>
//        friend std::ostream& operator<<(std::ostream& os, const Buffer<TT, s, ITT>& b);

    Buffer(): iter() {}

    template<typename U>
    inline void push(U&& value)
    {
        if(isNotFull()) {
            data[iter.getInputPosition()] = value;
            iter.nextInput();
        }
    }

    template<typename U>
    inline typename std::remove_reference<U>::type pop()
    {
        typename std::remove_reference<U>::type value{};
        if(isNotEmpty()){
            value = std::move(data[iter.getOutputPosition()]);
            iter.nextOutput();
        }
        return value;
    }

    inline void clear()
    {
        iter.reset();
    }

    inline const T& operator[](index_t i) const
    {
        return data[iter.getIndexByCount(i)];
    }

    inline T& operator[](index_t i)
    {
        return data[iter.getIndexByCount(i)];
    }

    inline index_t size() const
    {
        return iter.size();
    }

    inline bool isNotEmpty() const
    {
        return iter.isNotFloor();
    }

    inline bool isNotFull() const
    {
        return iter.isNotCeil();
    }

    // Additional operators
    template<typename U>
    inline Buffer& operator<<(U&& value)
    {
        push(std::forward<U>(value));
        return *this;
    }

    template<typename U>
    inline Buffer& operator>>(U&& value)
    {
        value = pop<U>();
        return *this;
    }
private:
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    IT iter;
    T data[SIZE];
};
