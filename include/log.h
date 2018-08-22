#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <fstream>
#include <iostream>

using namespace std;

enum LogLevel {
    ERROR, WARNING, INFO, DEBUG
};
class Log {
public:
    Log(bool outConsole, string outFileName = "NULL");

    void setLevel(LogLevel level);

    void print(LogLevel level, string msg);
    void error(string msg);
    void warning(string msg);
    void info(string msg);
    void debug(string msg);
private:
    LogLevel currentLevel = LogLevel::INFO;
    bool doOutConsole = true;
    bool doOutFile = false;
    ofstream outFile;
};

#endif // LOG_H_INCLUDED
