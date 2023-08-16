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
        typename RT = void     // Handler return type
        >
class HandlerRecord {
public:
    typedef ER event_t;
    typedef typename ER::id_t id_t;
    typedef typename ER::param_t param_t;
    typedef RT return_t;
    typedef RT (*handler_t)(param_t);

    explicit HandlerRecord(): id(), handler(nullptr){}
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

    inline id_t getID() const
    {
        return id;
    }

    inline operator bool()
    {
        return handler != nullptr;
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

    inline void handle(const typename HR::event_t& event)
    {
        for(index_t i = lookup.size(); i ; i--){
            if (lookup[i - 1].corresponds(event)){
                lookup[i - 1](event);
                break;
            }
        }
    }

    inline void addHandler(const typename HR::id_t& id, const typename HR::handler_t& handler)
    {
        if (lookup.isNotFull()){
            lookup.push(HR(id, handler));
        }
    }

    inline void deleteLastHandler(const typename HR::id_t& id)
    {
        for(index_t i = lookup.size(); i ; i--){
            if (id == lookup[i - 1].getID()){
                for(index_t j = i; j < lookup.size(); j++){
                    lookup[j - 1] = lookup[j];
                }
                lookup.template pop<HR>();
                break;
            }
        }
    }

    // Additional operators
    inline StackHandlerMapper& operator<<(const HR& record)
    {
        if (lookup.isNotFull()){
            lookup.push(record);
        }
        return *this;
    }

    inline StackHandlerMapper& operator<<(const HR * recordsPtr)
    {
        while(HR currentRecord = *recordsPtr++){
            *this << currentRecord;
        }
        return *this;
    }

    inline StackHandlerMapper& operator>>(const typename HR::id_t& id)
    {
        deleteLastHandler(id);
        return *this;
    }

private:
    StackHandlerMapper(const StackHandlerMapper&) = delete;
    StackHandlerMapper& operator=(const StackHandlerMapper&) = delete;

    StackBuffer<HR, stackDepth> lookup;
};
