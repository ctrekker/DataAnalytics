#include <vector>
#include "dataio.h"

#ifndef DATA_ANALYZER_H_INCLUDED
#define DATA_ANALYZER_H_INCLUDED

namespace analyze {
    void create_patterns(dataio::Pattern*, dataio::Graph);
    void train(std::vector<dataio::Match>, dataio::Graph, dataio::Pattern[]);
    void predict();
    void patternMatch(std::vector<dataio::Match>*, dataio::Graph, dataio::Pattern, bool);
    void patternMatch(std::vector<dataio::Match>*, dataio::Graph, dataio::Pattern, int, int, bool);
}

#endif // DATA_ANALYZER_H_INCLUDED
