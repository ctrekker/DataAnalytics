#include <iostream>

#ifndef STATE_H
#define STATE_H

using namespace std;

namespace state {
    extern string metaPath;
    extern uint64_t totalPatterns;
    extern uint64_t initTotalPatterns;
    extern uint64_t runId;

    void init(bool printState=true);
    void preserve();
    void print(bool pretty=false);

    uint64_t getPatternId(uint64_t);
    int getFileId(uint64_t);
    string getFilePath(uint64_t);
    bool patternFileExists(int);
    bool matchFileExists(int);
}

#endif /* STATE_H */

