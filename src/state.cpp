#include <iostream>
#include <fstream>
#include <string>

#include "config.h"
#include "state.h"
#include "log.h"

using namespace std;

extern Log LOG;

namespace state {
    string metaPath = SAVE_DIR + "/meta.txt";
    uint64_t totalPatterns = 0;
    uint64_t initTotalPatterns = 0;

    inline bool file_exists(string name) {
        ifstream file(name.c_str());
        return file.good();
    }
    void init(bool printState) {
        if(!file_exists(metaPath)) {
            ofstream metaCreate(metaPath);
            metaCreate << 0 << " ";
            metaCreate << 0 << " ";
            metaCreate.close();
        }

        ifstream metaIn(metaPath);

        string totalPatternsStr;
        metaIn >> totalPatternsStr;

        totalPatterns = stoi(totalPatternsStr);
        initTotalPatterns = totalPatterns;

        if(printState) {
            print();
        }

        metaIn.close();
    }
    void preserve() {
        ofstream metaOut(metaPath);

        print();

        metaOut << totalPatterns << " ";

        metaOut.close();
    }
    void print(bool pretty) {
        if(pretty) {
            LOG.debug("Total Saved Patterns: " + to_string(totalPatterns));
        }
        else {
            LOG.debug("State:");
            LOG.debug("\ttotalPatterns = " + to_string(totalPatterns));
        }
    }

    uint64_t getPatternId(uint64_t off) {
        return totalPatterns + off;
    }
    int getFileId(uint64_t pid) {
        return pid/OBJ_PER_FILE;
    }
    string getFilePath(uint64_t pid) {
        return SAVE_DIR+"/"+to_string(getFileId(pid))+".pbin";
    }
    bool patternFileExists(int fid) {
        return file_exists(SAVE_DIR+"/"+to_string(fid)+".pbin");
    }
    bool matchFileExists(int fid) {
        return file_exists(SAVE_DIR+"/"+to_string(fid)+".mbin");
    }
}
