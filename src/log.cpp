#include "log.h"
#include <fstream>
#include <iostream>

using namespace std;

Log::Log(bool outConsole, string outFileName) {
    this->doOutConsole = outConsole;
    this->doOutFile = (outFileName != "NULL");
    if(this->doOutFile) {
        this->outFile = ofstream(outFileName);
    }
}
