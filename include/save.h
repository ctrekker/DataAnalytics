#ifndef SAVE_H_INCLUDED
#define SAVE_H_INCLUDED

#include <fstream>
#include "dataio.h"

using namespace dataio;

namespace save {
    void createdPatterns(vector<Pattern>*);
    void createdMatches(vector<MatchList>*, int, bool = false);

    void state(string, vector<Pattern>*, vector<MatchList>*, vector<Prediction>*);

    void pattern(vector<uint8_t>*, Pattern*);
    void matchList(vector<uint8_t>*, MatchList*);
    void match(vector<uint8_t>*, Match*);
    void prediction(vector<uint8_t>*, Prediction*);

    void patternList(vector<Pattern>*, ofstream*);
    void matchListCollection(vector<MatchList>*, ofstream*);
    void predictionList(vector<Prediction>*, ofstream*);
    void csvPredictionList(Graph*, vector<Prediction>*, ofstream &);

    void buffToFile(vector<uint8_t>*, ofstream*);
}

#endif // SAVE_H_INCLUDED
