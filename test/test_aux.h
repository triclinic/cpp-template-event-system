#pragma once

#include <sstream>
#include <iostream>

template<class T = int, std::ostream *OS = nullptr>
class SemanticsChecker {
    T val;
    std::ostream *os;
    unsigned _copyConstructedTimes;
    unsigned _moveConstructedTimes;
    unsigned _copyAssignedTimes;
    unsigned _moveAssignedTimes;

    void logPrint(const char* mess)
    {
        if(os)
        {
            (*os)
                << "Called "
                << mess
                << " with value: "
                << val
                << std::endl;
        }
    }
    void logPrint(const char* mess, const SemanticsChecker& other)
    {
        if(os)
        {
            (*os)
                << "Called "
                << mess
                << ": "
                << val
                << " on "
                << other.val
                << std::endl;
        }
    }
public:

    SemanticsChecker(T val = T{}):
                                val(val),
                                os(OS),
                                _copyConstructedTimes(0u),
                                _moveConstructedTimes(0u),
                                _copyAssignedTimes(0u),
                                _moveAssignedTimes(0u)
    {
        logPrint("default constructor");
    }
    SemanticsChecker(const SemanticsChecker& other):
                                val(other.val),
                                os(OS),
                                _copyConstructedTimes(other._copyConstructedTimes + 1),
                                _moveConstructedTimes(other._moveConstructedTimes),
                                _copyAssignedTimes(other._copyAssignedTimes),
                                _moveAssignedTimes(other._moveAssignedTimes)
    {
        logPrint("copy constructor");
    }
    SemanticsChecker(SemanticsChecker&& other):
                                val(std::move(other.val)),
                                os(OS),
                                _copyConstructedTimes(other._copyConstructedTimes),
                                _moveConstructedTimes(other._moveConstructedTimes + 1),
                                _copyAssignedTimes(other._copyAssignedTimes),
                                _moveAssignedTimes(other._moveAssignedTimes)
    {
        logPrint("move constructor");
    }

    SemanticsChecker& operator=(const SemanticsChecker& other)
    {
        if(this != &other) {
            logPrint("copy assignment", other);
            val = other.val;
            _copyConstructedTimes = other._copyConstructedTimes;
            _moveConstructedTimes = other._moveConstructedTimes;
            _copyAssignedTimes = other._copyAssignedTimes + 1;
            _moveAssignedTimes = other._moveAssignedTimes;
        }
        return *this;
    }
    SemanticsChecker& operator=(SemanticsChecker&& other)
    {
        if(this != &other) {
            logPrint("move assignment", other);
            val = std::move(other.val);
            _copyConstructedTimes = other._copyConstructedTimes;
            _moveConstructedTimes = other._moveConstructedTimes;
            _copyAssignedTimes = other._copyAssignedTimes;
            _moveAssignedTimes = other._moveAssignedTimes + 1;
        }
        return *this;
    }
    ~SemanticsChecker() {
        logPrint("destructor");
    }

    bool operator==(const SemanticsChecker& other) const
    {
        return val == other.val;
    }

    operator T() const
    {
        return (val);
    }


    ////////////////////////////////////////////////////
    bool isCopyConstructed() const
    {
        return _copyConstructedTimes != 0;
    }

    bool isCopyAssigned() const
    {
        return _copyAssignedTimes != 0;
    }

    bool isCopied() const
    {
        return isCopyConstructed() || isCopyAssigned();
    }

    /////////////////////////////////////////////////////
    unsigned copyConstructedTimes() const
    {
        return _copyConstructedTimes;
    }

    unsigned copyAssignedTimes() const
    {
        return _copyAssignedTimes;
    }

    unsigned copiedTimes() const
    {
        return copyConstructedTimes() + copyAssignedTimes();
    }

    /////////////////////////////////////////////////////
    bool isMoveConstructed() const
    {
        return _moveConstructedTimes != 0;
    }

    bool isMoveAssigned() const
    {
        return _moveAssignedTimes != 0;
    }

    bool isMoved() const
    {
        return isMoveConstructed() || isMoveAssigned();
    }

    //////////////////////////////////////////////////////
    unsigned moveConstructedTimes() const
    {
        return _moveConstructedTimes;
    }

    unsigned moveAssignedTimes() const
    {
        return _moveAssignedTimes;
    }

    unsigned movedTimes() const
    {
        return moveConstructedTimes() + moveAssignedTimes();
    }

    ///////////////////////////////////////////////////////
    std::stringstream getOrder() const
    {
        std::stringstream output;
        output  << "Value (" << val << ") "
                << "copy constructed " << copyConstructedTimes() << " times, "
                << "copy assigned " << copyAssignedTimes() << " times, "
                << "move constructed " << moveConstructedTimes() << " times, "
                << "move assigned " << moveAssignedTimes() << " times. ";
        return output;
    }
};

typedef SemanticsChecker<> semantics_t;
