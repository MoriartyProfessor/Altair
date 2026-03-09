#ifndef GWAIHIR_TIMER_HPP
#define GWAIHIR_TIMER_HPP

#include <chrono>
#include <exception>

namespace 
{
    using namespace std::chrono;
};

class Timer
{
    public:
    void start()
    {
        started = true;
        stopped = false;
        begin = high_resolution_clock::now();
    }
    void stop()
    {
        stopped = true;
        end = high_resolution_clock::now();
    }
    auto duration() -> duration<double, std::milli>
    {
        if(!(started && stopped))
            throw std::runtime_error("Timer was not started or has not stopped");
        return end - begin;
    }
    private:
    bool started = false;
    bool stopped = false;
    time_point<system_clock> begin;
    time_point<system_clock> end;
};

#endif // GWAIHIR_TIMER_HPP