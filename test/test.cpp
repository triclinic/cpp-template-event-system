#include <iostream>
#include <vector>
#include "event.h"
#include "stack.h"
#include "circular.h"

template<class T, unsigned s, class IT>
std::ostream& operator<<(std::ostream& os, const Buffer<T, s, IT>& b)
{
    os << "Buffer repr: [";
    for(unsigned i = 0; i < b.size(); ++i){
        os << b[i] << ", ";
    }
    os << "]" << std::endl;
    os << "Count: " << (unsigned)b.size() << std::endl;
    return os;
}

enum class vfoEvents {
    KEYPAD,
    UPDATE,
    EMPTY
};

template<class T = int, std::ostream *OS = nullptr>
class InvokeLogger {
    T val;
    std::ostream *os;
    unsigned flags;

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
    void logPrint(const char* mess, const InvokeLogger& other)
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
    enum class Method : unsigned {
        DEFAULT_CTOR,
        COPY_CTOR,
        MOVE_CTOR,
        COPY_ASSIGN,
        MOVE_ASSIGN,
    };

    InvokeLogger(T val = T{}): val(val), os(OS), flags(0u)
    {
        logPrint("default constructor");
        flags |= (1u << (unsigned)Method::DEFAULT_CTOR);
    }
    InvokeLogger(const InvokeLogger& other): val(other.val), os(OS), flags(0u)
    {
        logPrint("copy constructor");
        flags |= (1u << (unsigned)Method::COPY_CTOR);
    }
    InvokeLogger(InvokeLogger&& other): val(std::move(other.val)), os(OS), flags(0u)
    {
        logPrint("move constructor");
        flags |= (1u << (unsigned)Method::MOVE_CTOR);
    }

    InvokeLogger& operator=(const InvokeLogger& other)
    {
        if(this != &other) {
            logPrint("copy assignment", other);
            flags |= (1u << (unsigned)Method::COPY_ASSIGN);
            val = other.val;
        }
        return *this;
    }
    InvokeLogger& operator=(InvokeLogger&& other)
    {
        if(this != &other) {
            logPrint("move assignment", other);
            flags |= (1u << (unsigned)Method::MOVE_ASSIGN);
            val = std::move(other.val);
        }
        return *this;
    }
    ~InvokeLogger() {
        logPrint("destructor");
    }


    operator T() const
    {
        return (val);
    }
    bool operator==(const InvokeLogger& other) const
    {
        return val == other.val;
    }

    bool isCalled(Method m)
    {
        return (flags & (1u << (unsigned)m)) != 0;
    }
    bool isNotCalled(Method m)
    {
        return (flags & (1u << (unsigned)m)) == 0;
    }
    void clearCalledFlag(Method m)
    {
        flags &= ~(1u << (unsigned)m);
    }
};


int test_handler(int val){
    return -55 + val;
}


int main()
{
    using namespace std;

	cout << "Test!!!" << endl;

    typedef InvokeLogger<int> logger_t;
    typedef InvokeLogger<int>::Method Method;
    StackBuffer<logger_t, 10> buf;


    vector<logger_t> test = {2, -336, 0, 2368, 1246, -2178, 56329, 42, 22, 55, 987};
    cout << "^^^^^^^^^^^^^^^^^^^^^^" << buf << endl;
    test[2] = 8888;
    for(const logger_t& v: test){
        buf << v;
    }
    cout << "^^^^^^^^^^^^^^^^^^^^^^" << buf << endl;
    logger_t v;
    buf >> v >> v >> v;
    buf << 145 << -777 << -888;
    cout << "^^^^^^^^^^^^^^^^^^^^^^" << buf << endl;
    for(unsigned i = 0; i < test.size(); ++i){
        logger_t l;
        buf >> l;
        cout
            << (l == test[i])
            << l.isNotCalled(Method::COPY_ASSIGN)
            << l.isCalled(Method::MOVE_ASSIGN)
            << test[i].isNotCalled(Method::COPY_ASSIGN)
            << test[i].isNotCalled(Method::MOVE_ASSIGN)
            << endl;
    }
    typedef EventRecord<vfoEvents, int> vfoEventRecord;
    typedef HandlerRecord<vfoEventRecord, int> vfoHandlerRecord;
    typedef StackHandlerMapper<vfoHandlerRecord, 32u> vfoHandlerMapper;
    typedef EventLoop<vfoEventRecord, vfoHandlerMapper> vfoEventLoop;

    vfoEventRecord evk(vfoEvents::KEYPAD, 3), eve(vfoEvents::EMPTY, 3);
    vfoHandlerRecord hr(vfoEvents::EMPTY, test_handler);
    vfoEventLoop app;

    //cout << "%%%" << hr(evk)<< "%%%" << hr(eve);

    app.sendEvent(vfoEvents::KEYPAD, 3);
    app.getMapper().addHandler(vfoEvents::KEYPAD, test_handler);

    app.exec();
    return 0;
}
