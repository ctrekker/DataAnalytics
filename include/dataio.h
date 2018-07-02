#ifndef PATTERN_H_INCLUDED
#define PATTERN_H_INCLUDED

#include <vector>
#include <sstream>
#include "dataio.h"
#include "config.h"

using namespace std;

namespace dataio {
    struct Graph {
    public:
        int timeIndex;
        short int dimensions;
        vector<vector<double>> data;

        void init(short int dimensions, int timeIndex, vector<vector<double>> data) {
            this->dimensions = dimensions;
            this->timeIndex = timeIndex;
            this->data = data;
        }
        string toString() {
            stringstream out;
            out << "Graph[" << this << "]" << endl;
            out << "\tdimensions: " << dimensions << endl;
            out << "\ttimeIndex: " << timeIndex << endl;
            out << "\tdata: {";
            for(unsigned int x=0; x<data.size(); x++) {
                out << "{";
                for(unsigned int y=0; y<data[x].size(); y++) {
                    out << data[x][y];
                    if(y!=data[x].size()-1) out << ", ";
                }
                out << ((x!=data.size()-1) ? "}, " : "}");
            }
            out << "}" << endl;

            return out.str();
        }
    };
    struct Pattern {
    public:
        uint64_t id;
        uint16_t dimensions;
        uint64_t created;
        uint32_t length;
        uint32_t corrections;
        double averageError;
        
        vector<double> body[PATTERN_LENGTH];
        vector<double> resultBody[PATTERN_LENGTH];
        
        string toString() {
            stringstream out;
            out << "Pattern[" << this << "]" << endl;
            out << "\tid: " << id << endl;
            out << "\tdimensions: " << dimensions << endl;
            out << "\tcreated: " << created << endl;
            out << "\tlength: " << length << endl;
            out << "\tcorrections: " << corrections << endl;
            out << "\taverageError: " << averageError << endl;
            out << "\tbody: {";
            for(unsigned int x=0; x<length; x++) {
                out << "{";
                for(unsigned int y=0; y<body[x].size(); y++) {
                    out << body[x][y];
                    if(y!=body[x].size()-1) out << ", ";
                }
                out << ((x!=length-1) ? "}, " : "}");
            }
            out << "}" << endl;
            out << "\tresultBody: {";
            for(unsigned int x=0; x<length; x++) {
                out << "{";
                for(unsigned int y=0; y<resultBody[x].size(); y++) {
                    out << resultBody[x][y];
                    if(y!=resultBody[x].size()-1) out << ", ";
                }
                out << ((x!=length-1) ? "}, " : "}");
            }
            out << "}" << endl;
            
            return out.str();
        }
    };
    struct Match {
    public:
        uint16_t length;
        double error;
        double data[PATTERN_LENGTH];
        
        double slopeIntercept;
        double translation;
        double patternZero;
    };
    struct MatchList {

    };
    struct Prediction {

    };
}

#endif // PATTERN_H_INCLUDED
