#pragma once
#include "circular.h"

enum class vfoEvents {
    KEYPAD,
    UPDATE
};

template<
         typename ET = vfoEvents,
         typename PT = unsigned char>
struct EventRecord {
    ET eventID;
    PT parameter;
public:
    typedef PT param_t;
    typedef ET event_t;
    EventRecord(){}
    EventRecord(ET eventID, PT parameter): eventID(eventID), parameter(parameter) {}
};

template<
         typename ET = vfoEvents,
         typename PT = unsigned char>
class HandlerRecord {
    ET eventID;
    void (*handler)(PT);
public:
    typedef PT param_t;
    typedef ET event_t;
    HandlerRecord():handler(nullptr){}
    HandlerRecord(ET eventID, void (*handler)(PT)): eventID(eventID), handler(handler){}

    inline void operator()(PT par) const
    {
        if(handler) handler(par);
    }
    inline ET getID() const
    {
        return eventID;
    }
};



template<typename EV = EventRecord<>,
         //typename HM = StackHandlerMapper<>,
         //typename BG = BackgroundTaskPoller<>,
         unsigned eventQueueSize = 32>
class EventLoop {
    //HM mapper;
    CircularBuffer<EV, eventQueueSize> systemQueue;

public:
    void send(EV&& e)
    {
        systemQueue.write(std::move(e));
    }

    void exec()
    {

    }
};
