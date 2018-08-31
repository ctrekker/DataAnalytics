#include "log.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <time.h>

using namespace std;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

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

    out << "[" << currentDateTime() << "][" << levelTag << levelSpaces << "]: " << msg << endl;


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
