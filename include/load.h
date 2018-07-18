#ifndef LOAD_H_INCLUDED
#define LOAD_H_INCLUDED

using namespace std;

namespace load {
    void pattern(Pattern*, vector<uint8_t>*);
    void match(Match*, vector<uint8_t>*);
    void matchList(MatchList*, vector<uint8_t>*);
    void prediction(Prediction*, vector<uint8_t>*);
}

#endif // LOAD_H_INCLUDED
