#include <iostream>
#include <fstream>
#include <string>

#include "config.h"
#include "state.h"

using namespace std;

namespace state {
    string metaPath = SAVE_DIR + "/meta.txt";
    uint64_t totalPatterns = 0;
    uint64_t initTotalPatterns = 0;
    
    inline bool file_exists(string name) {
        ifstream file(name.c_str());
        return file.good();
    }
    void init() {
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
        
        print();

        metaIn.close();
    }
    void preserve() {
        ofstream metaOut(metaPath);
        
        print();
        
        metaOut << totalPatterns << " ";
        
        metaOut.close();
    }
    void print() {
        cout << "State:" << endl;
        cout << "\ttotalPatterns = " << totalPatterns << endl;
        cout << endl;
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