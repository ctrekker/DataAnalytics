#include "log.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

Log::Log(bool outConsole, string outFileName) {
    this->doOutConsole = outConsole;
    this->doOutFile = (outFileName != "NULL");
    if(this->doOutFile) {
        this->outFile = ofstream(outFileName);
    }
}

void Log::print(LogLevel level, string msg) {
    stringstream out;

    string levelTag;
    switch(level) {
        case ERROR:
            levelTag = "ERROR";
            break;
        case WARNING:
            levelTag = "WARN";
            break;
        case INFO:
            levelTag = "INFO";
            break;
        case DEBUG:
            levelTag = "DEBUG";
            break;
    }
    string levelSpaces = "";
    while(levelSpaces.length()<5-levelTag.length()) {
        levelSpaces += " ";
    }

    out << "[" << levelTag << levelSpaces << "]: " << msg << endl;


    // Handle all outputs
    if(this->doOutConsole) {
        cout << out.str();
    }
    if(this->doOutFile) {
        this->outFile << out.str();
    }
}
void Log::error(string msg) {
    this->print(LogLevel::ERROR, msg);
}
void Log::warning(string msg) {
    this->print(LogLevel::WARNING, msg);
}
void Log::info(string msg) {
    this->print(LogLevel::INFO, msg);
}
void Log::debug(string msg) {
    this->print(LogLevel::DEBUG, msg);
}
