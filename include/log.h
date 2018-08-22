#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <fstream>
#include <iostream>

using namespace std;

class Log {
public:
    Log(bool outConsole, string outFileName = "NULL");


private:
    bool doOutConsole = true;
    bool doOutFile = false;
    ofstream outFile;
};

#endif // LOG_H_INCLUDED
