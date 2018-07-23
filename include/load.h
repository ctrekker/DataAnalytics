#ifndef LOAD_H_INCLUDED
#define LOAD_H_INCLUDED

#include <fstream>
#include "dataio.h"

using namespace std;
using namespace dataio;

namespace load {
    void state(string, vector<Pattern>*, vector<MatchList>*, vector<Prediction>*);

    int pattern(Pattern*, vector<uint8_t>*, uint64_t = 0);
    int match(Match*, vector<uint8_t>*, uint64_t = 0);
    int matchList(MatchList*, vector<uint8_t>*, uint64_t = 0);
    int prediction(Prediction*, vector<uint8_t>*, uint64_t = 0);

    void patternList(ifstream*, vector<Pattern>*, uint64_t = 0);
    void matchListCollection(ifstream*, vector<MatchList>*);
    void predictionList(ifstream*, vector<Prediction>*);

    void fileToBuff(ifstream*, vector<uint8_t>*);
}

#endif // LOAD_H_INCLUDED
