#ifndef GWAIHIR_TIMEMANAGER_HPP
#define GWAIHIR_TIMEMANAGER_HPP

#include "timer.hpp"

namespace 
{
    constexpr uint32_t MARGIN = 100; 
};

class TimeManager
{
    public:
    TimeManager(uint32_t inc, uint32_t time) 
    : inc_{inc}, time_{time}
    {
        if(time == 0)
            movetime_ = inc;
        else if(inc < time)
            movetime_ = inc + time / 40;
        else
            movetime_ = time / 40;
        timer_.start();
    }
    bool is_over()
    {
        return timer_.duration().count() > movetime_ - MARGIN;
    }

    private:
    const uint32_t inc_ = 0u;
    const uint32_t time_ = 0u;
    uint32_t movetime_;
    Timer<> timer_;
};

#endif // GWAIHIR_TIMEMANAGER_HPP