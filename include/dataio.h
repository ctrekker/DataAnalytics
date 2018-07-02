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

    };
    struct Match {

    };
    struct MatchList {

    };
    struct Prediction {

    };
}

#endif // PATTERN_H_INCLUDED
