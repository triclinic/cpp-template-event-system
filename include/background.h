#pragma once
#include "stack.h"


template<unsigned SIZE>
class BackGroundPoller{
public:
    typedef void (*task_t)(void);
    typedef typename StackBuffer<task_t, SIZE>::index_t index_t;

    template<unsigned S>
    static inline void poll()
    {
        std::cout << "Polling!!!" << std::endl;
        for(index_t i = 0; i < BackGroundPoller<S>::tasks.size(); i++){
            //tasks[i]();
        }
    }
    static inline void addBackgroundTask(task_t task)
    {
        //tasks.push(task);
    }

private:
    static StackBuffer<task_t, SIZE> tasks;
};
