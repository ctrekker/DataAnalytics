#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

namespace timer {
    void start();
    void stop(std::string);
    uint64_t getTimeMillis();
}

#endif // TIMER_H_INCLUDED
