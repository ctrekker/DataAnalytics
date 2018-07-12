#include <vector>
#include "dataio.h"

#ifndef DATA_ANALYZER_H_INCLUDED
#define DATA_ANALYZER_H_INCLUDED

namespace analyze {
    void create_patterns(std::vector<dataio::Pattern> &patterns, dataio::Graph);
    void train(std::vector<dataio::MatchList>*, std::vector<dataio::Pattern>, dataio::Graph);
    bool predict(std::vector<dataio::Prediction>*, std::vector<dataio::Pattern> &patterns, std::vector<dataio::MatchList>, dataio::Graph, uint64_t, uint64_t, int, int);
    void patternMatch(std::vector<dataio::Match>*, dataio::Graph, dataio::Pattern, bool);
    void patternMatch(std::vector<dataio::Match>*, dataio::Graph, dataio::Pattern, int, int, bool);
    void getMatchesByPid(std::vector<dataio::Match>*, std::vector<dataio::Match>*, uint64_t);

    double bellCurve(double);
    double bellCurve(double, double, double);
    vector<double> sequenceTransform(std::vector<double>, std::vector<double>);
    vector<double> sequenceTranslate(double, std::vector<double>);
    vector<double> combineSequence(std::vector<double>, std::vector<double>);
    void printVector(std::vector<double>);
    void getMatchesByPid(std::vector<dataio::Match>*, std::vector<dataio::Match>*, uint64_t);
}

#endif // DATA_ANALYZER_H_INCLUDED
