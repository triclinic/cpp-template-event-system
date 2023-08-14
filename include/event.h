#pragma once
#include "circular.h"
#include "handler.h"

template<typename EV,
         typename HM,
         //typename BG = BackgroundTaskPoller<>,
         unsigned eventQueueSize = 32u>
class EventLoop {

public:
    EventLoop() = default;

    template<typename ER>
    inline void sendEvent(ER&& er)
    {
        systemQueue.push(er);
    }

    template<typename E, typename P>
    inline void sendEvent(E&& e, P&& p)
    {
        systemQueue.push(EV(e, p));
    }

    inline void exec()
    {
        for(;;){
            if(systemQueue.isNotEmpty()){
                EV currentEvent = systemQueue.template pop<EV>();
                mapper.handle(currentEvent);
                break; //FIXME: For debug
            } else {
                break;
            }
            //TODO: Execute background tasks
        }
    }

    inline HM& getMapper()
    {
        return mapper;
    }

private:
    EventLoop(const EventLoop&) = delete;

    HM mapper;
    CircularBuffer<EV, eventQueueSize> systemQueue;
};
