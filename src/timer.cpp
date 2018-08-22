#include <iostream>
#include <chrono>
#include <sstream>

#include "log.h"

using namespace std;

extern Log LOG;

namespace timer {
    chrono::time_point<chrono::high_resolution_clock> timer_started;
    void start() {
        timer_started=chrono::high_resolution_clock::now();
    }
    void stop(string str) {
        auto timer_stopped=chrono::high_resolution_clock::now();
        int64_t time = std::chrono::duration_cast<std::chrono::nanoseconds>(timer_stopped-timer_started).count();
        string label = "ns";
        if(time>10000) {
            time/=1000;
            label="us";
        }
        if(time>10000) {
            time/=1000;
            label="ms";
        }
        if(time>10000) {
            time/=1000;
            label="s";

            if(time>120) {
                time/=60;
                label="m";
            }
            if(time>120) {
                time/=60;
                label="h";
            }
        }
        stringstream ss;
        ss << str << " in " << time << label;
        LOG.info(ss.str());
    }
    uint64_t getTimeMillis() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
    }
}
