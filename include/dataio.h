#ifndef PATTERN_H_INCLUDED
#define PATTERN_H_INCLUDED

#include <vector>
#include <sstream>
#include <float.h>
#include "dataio.h"
#include "config.h"

using namespace std;

extern Config C;

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
        Graph clip(int position, int length) {
            vector<vector<double>> clippedData(length, vector<double>(data[0].size()));

            for(int i=position; i<position+length; i++) {
                for(unsigned int j=0; j<data[i].size(); j++) {
                    clippedData[i-position][j] = data[i][j];
                }
            }

            Graph g;
            g.init(dimensions, timeIndex, clippedData);
            return g;
        }
        vector<double> getPlotArray(int index) {
            vector<double> out = *new vector<double>(data.size());

            for(unsigned int i=0; i<data.size(); i++) {
                out[i] = data[i][index];
            }

            return out;
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
        /*** NOTE: this value must be changed if new fields are added ***/
        const static int SIZE = 8+2+8+4+4+8+(8*PATTERN_LENGTH*DIMENSIONS)*2;

        uint64_t id;
        uint16_t dimensions;
        uint64_t created;
        uint32_t length;
        uint32_t corrections;
        double averageError;

        vector<vector<double>> body;
        vector<vector<double>> resultBody;

        vector<double> getResultBodyDimension(int dim) {
            vector<double> out = *new vector<double>(PATTERN_LENGTH);
            for(unsigned int i=0; i<PATTERN_LENGTH; i++) {
                out[i] = resultBody[i][dim];
            }
            return out;
        }
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
        /*** NOTE: this value must be changed if new fields are added ***/
        const static int SIZE = 2+8+(8*MATCH_MAX_DATA_SIZE)+8+8+8;
        uint64_t pid = 0;
        uint16_t length = 0;
        double error = DBL_MAX;
        vector<double> data = vector<double>(MATCH_MAX_DATA_SIZE);

        double slopeIntercept = 0;
        double translation = 0;
        double patternZero = 0;

        vector<double> translateData(vector<double> body) {
            vector<double> out = *new vector<double>(body.size());
            for(unsigned int i=0; i<body.size(); i++) {
                out[i] = (slopeIntercept * (body[i] - patternZero) + patternZero + translation);
            }
            return out;
        }
        string toString() {
            stringstream out;
            out << "Match[" << this << "]" << endl;
            out << "\tpid: " << pid << endl;
            out << "\tlength: " << length << endl;
            out << "\terror: " << error << endl;
            out << "\tslopeIntercept: " << slopeIntercept << endl;
            out << "\ttranslation: " << translation << endl;
            out << "\tpatternZero: " << patternZero << endl;
            out << "\tresultData: {";
            for(unsigned int i=0; i<PATTERN_LENGTH; i++) {
                out << data[i] << ((i==PATTERN_LENGTH-1)?"":",");
            }
            out << "}" << endl;

            return out.str();
        }
    };
    struct MatchList {
    public:
        /*** NOTE: this value must be changed if new fields are added ***/
        const static int SIZE = 8+2+4+(MATCH_BUFFER_SIZE*Match::SIZE);
        uint64_t id;
        uint16_t currentIndex = 0;
        uint32_t totalMatches = 0;
        vector<Match> matches;

        void addMatch(Match m, bool over = false) {
            if(!over) {
                while(matches.size()<MATCH_BUFFER_SIZE) {
                    matches.push_back(*(new Match));
                }
                matches[currentIndex]=m;
                currentIndex++;
                if(currentIndex>=MATCH_BUFFER_SIZE) {
                    currentIndex=0;
                }
                totalMatches++;
            }
            else {
                matches.push_back(m);
            }
        }
        string toString() {
            stringstream out;
            out << "-----MatchList[" << this << "]-----" << endl;
            out << "\tid: " << id << endl;
            out << "\tcurrentIndex: " << currentIndex << endl;
            out << "\ttotalMatches: " << totalMatches << endl;
            out << "\tmatches: " << endl;
            for(unsigned int i=0; i<matches.size(); i++) {
                out << matches[i].toString();
            }

            return out.str();
        }
    };
    struct Prediction {
    public:
        uint64_t patternId;
        uint16_t matchIndex;
        vector<double> result;
        double bellWeight;
        double patternPercentage;

        void init(uint64_t patternId, uint16_t matchIndex, vector<double> result, double bellWeight, double patternPercentage) {
            this->patternId = patternId;
            this->matchIndex = matchIndex;
            this->result = result;
            this->bellWeight = bellWeight;
            this->patternPercentage = patternPercentage;
        }
        bool containedIn(vector<Prediction>* predictions) {
            for(unsigned int i=0; i<predictions->size(); i++) {
                Prediction current = (*predictions)[i];
                if(this->equals(current)) {
                    return true;
                }
            }
            return false;
        }
        bool equals(Prediction &other) {
            if(this->result.size()!=other.result.size()) return false;
            for(unsigned int i=0; i<this->result.size(); i++) {
                if(this->result[i] != other.result[i]) return false;
            }
            return true;
        }
        string toString() {
            stringstream out;
            out << "Prediction[" << this << "]" << endl;
            out << "\tpatternId: " << patternId << endl;
            out << "\tmatchIndex: " << matchIndex << endl;
            out << "\tbellWeight: " << bellWeight << endl;
            out << "\tpatternPercentage: " << patternPercentage << endl;
            out << "\tresult: {";
            for(unsigned int i=0; i<result.size(); i++) {
                out << result[i] << ((i==result.size()-1)?"":",");
            }
            out << "}" << endl;

            return out.str();
        }
    };
}

#endif // PATTERN_H_INCLUDED
