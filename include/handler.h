#pragma once
#include "stack.h"

template<
         typename ET,
         typename PT
         >
struct EventRecord {
public:
    typedef ET id_t;
    typedef PT param_t;

    EventRecord(){}
    EventRecord(ET id, PT parameter): id(id), parameter(parameter) {}

    ET id;
    PT parameter;
};


template<
        typename ER,
        typename RT = void
        >
class HandlerRecord {
public:
    typedef ER event_t;
    typedef typename ER::id_t id_t;
    typedef typename ER::param_t param_t;
    typedef RT return_t;
    typedef RT (*handler_t)(param_t);

    explicit HandlerRecord():handler(nullptr), id(){}
    HandlerRecord(id_t id, handler_t handler): id(id), handler(handler){}

    template<typename U = RT>
    typename std::enable_if<std::is_void<U>::value>::type
    inline operator()(const ER& event) const
    {
        if(this->corresponds(event) && handler){
            handler(event.parameter);
        }
    }

    template<typename U = RT>
    typename std::enable_if<!std::is_void<U>::value, U>::type
    inline operator()(const ER& event) const
    {
        if(this->corresponds(event) && handler){
            return handler(event.parameter);
        }
        return U{};
    }

    inline bool corresponds(const ER& event) const
    {
        return (event.id == id);
    }

    inline void bind(const id_t& id, const handler_t& handler)
    {
        this->id = id;
        this->handler = handler;
    }

    inline id_t getID() const
    {
        return id;
    }
private:
    id_t id;
    handler_t handler;
};


template<
        typename HR,
        unsigned stackDepth = 32
        >
class StackHandlerMapper{
    typedef typename StackBuffer<HR, stackDepth>::index_t index_t;
public:

    StackHandlerMapper() = default;

    void handle(const typename HR::event_t& event)
    {
        if (lookup.isNotEmpty()){
            for(index_t i = lookup.size(); i ; i--){
                if (lookup[i - 1].corresponds(event)){
                    lookup[i - 1](event);
                    break;
                }
            }
        }
    }
    void addHandler(const typename HR::id_t& id, const typename HR::handler_t& handler)
    {
        if (lookup.isNotFull()){
            lookup.push(HR(id, handler));
        }
    }
    void deleteLastHandler(const typename HR::id_t& id)
    {
        //TODO: implement stacking
    }
private:
    StackHandlerMapper(const StackHandlerMapper&) = delete;

    StackBuffer<HR, stackDepth> lookup;
};
