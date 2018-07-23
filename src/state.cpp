#include <iostream>
#include <fstream>
#include <string>

#include "config.h"
#include "state.h"

using namespace std;

namespace state {
    bool saved = false;
    int currentSaveFile;
    uint64_t totalPatternNumber;

    inline bool file_exists(string name) {
        ifstream file(name.c_str());
        return file.good();
    }
    void init() {
        string metaPath = SAVE_DIR+"/meta.bin";
        if(!file_exists(metaPath)) {
            ofstream metaCreate(metaPath);
            metaCreate << 0 << " ";
            metaCreate << 0 << " ";
            metaCreate.close();
        }

        ifstream metaIn(metaPath);

        string totalPatternNumberStr;
        metaIn >> totalPatternNumberStr;
        string currentSaveFileStr;
        metaIn >> currentSaveFileStr;

        totalPatternNumber = stoi(totalPatternNumberStr);
        currentSaveFile = stoi(currentSaveFileStr);

        cout << "State variables:" << endl;
        cout << "\ttotalPatternNumber = " << totalPatternNumber << endl;
        cout << "\tcurrentSaveFileID = " << currentSaveFile << endl;
        cout << endl;

        metaIn.close();
    }
    void preserve() {
        string metaPath = SAVE_DIR+"/meta.bin";
        ofstream metaOut(metaPath);

        if(!saved) {
            totalPatternNumber += PATTERN_NUMBER;
        }
        metaOut << totalPatternNumber << " " << endl;
        metaOut << totalPatternNumber / PATTERN_SWAP_THRESHOLD << " " << endl;

        metaOut.close();
    }
}
