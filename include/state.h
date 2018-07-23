#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

namespace state {
    extern int currentSaveFile;
    extern uint64_t totalPatternNumber;

    void init();
    void preserve();
}

#endif // STATE_H_INCLUDED
