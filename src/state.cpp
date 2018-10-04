#include <iostream>
#include <fstream>
#include <string>

#include "config.h"
#include "state.h"
#include "log.h"

using namespace std;

extern Log LOG;
extern Config C;

namespace state {
    string metaPath = C.SAVE_DIR + "/meta.txt";
    uint64_t totalPatterns = 0;
    uint64_t initTotalPatterns = 0;
    uint64_t runId = 0;

    inline bool file_exists(string name) {
        ifstream file(name.c_str());
        return file.good();
    }
    void init(bool printState) {
        // Refresh metaPath in case C.SAVE_DIR has changed
        metaPath = C.SAVE_DIR + "/meta.txt";

        if(!file_exists(metaPath)) {
            ofstream metaCreate(metaPath);
            metaCreate << 0 << " ";
            metaCreate << 0 << " ";
            metaCreate.close();
        }

        ifstream metaIn(metaPath);

        string totalPatternsStr;
        metaIn >> totalPatternsStr;
        string runIdStr;
        metaIn >> runIdStr;

        totalPatterns = stoi(totalPatternsStr);
        initTotalPatterns = totalPatterns;

        runId = stoi(runIdStr);

        if(printState) {
            print();
        }

        metaIn.close();
    }
    void preserve() {
        ofstream metaOut(metaPath);

        metaOut << totalPatterns << " ";
        metaOut << runId+1 << " ";

        runId+=1;
        print();
        runId-=1;

        metaOut.close();
    }
    void print(bool pretty) {
        if(pretty) {
            LOG.debug("Total Saved Patterns: " + to_string(totalPatterns));
            LOG.debug("Run ID: " + to_string(runId));
        }
        else {
            LOG.debug("State:");
            LOG.debug("\ttotalPatterns = " + to_string(totalPatterns));
            LOG.debug("\trunId = " + to_string(runId));
        }
    }

    uint64_t getPatternId(uint64_t off) {
        return totalPatterns + off;
    }
    int getFileId(uint64_t pid) {
        return pid/C.OBJ_PER_FILE;
    }
    string getFilePath(uint64_t pid) {
        return C.SAVE_DIR+"/"+to_string(getFileId(pid))+".pbin";
    }
    bool patternFileExists(int fid) {
        return file_exists(C.SAVE_DIR+"/"+to_string(fid)+".pbin");
    }
    bool matchFileExists(int fid) {
        return file_exists(C.SAVE_DIR+"/"+to_string(fid)+".mbin");
    }
}
