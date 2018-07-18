#include <vector>
#include "dataio.h"
#include "stream.h"

using namespace std;
using namespace dataio;

namespace load {
    void pattern(Pattern* out, vector<uint8_t>* s) {
        uint64_t cp = 0;
        out->id = stream::readLong(s, &cp);
        out->dimensions = stream::readShort(s, &cp);
        out->created = stream::readLong(s, &cp);
        out->length = stream::readInt(s, &cp);
        out->corrections = stream::readInt(s, &cp);
        out->averageError = stream::readDouble(s, &cp);

        vector<vector<double>>* body;
        int it=0;
        do {
            if(it==0) body = &out->body;
            else body = &out->resultBody;

            for(unsigned int i=0; i<out->length; i++) {
                if((*body).size()<=i) {
                    body->push_back(vector<double>(out->dimensions));
                }
                for(unsigned int j=0; j<out->dimensions; j++) {
                    double d = stream::readDouble(s, &cp);
                    (*body)[i][j] = d;
                }
            }

            it++;
        } while(body!=&out->resultBody);
    }
    void match(Match* out, vector<uint8_t>* s) {
        uint64_t cp = 0;
        out->length = stream::readShort(s, &cp);
        out->error = stream::readDouble(s, &cp);
        out->slopeIntercept = stream::readDouble(s, &cp);
        out->translation = stream::readDouble(s, &cp);
        out->patternZero = stream::readDouble(s, &cp);
        for(unsigned int i=0; i<MATCH_MAX_DATA_SIZE; i++) {
            out->data[i] = stream::readDouble(s, &cp);
        }
    }
    void matchList(MatchList* out, vector<uint8_t>* stream) {

    }
    void prediction(Prediction* out, vector<uint8_t>* s) {
        uint64_t cp = 0;
        out->patternId = stream::readLong(s, &cp);
        out->matchIndex = stream::readShort(s, &cp);
        uint32_t resultSize = stream::readInt(s, &cp);
        out->bellWeight = stream::readDouble(s, &cp);
        out->patternPercentage = stream::readDouble(s, &cp);

        out->result = vector<double>(resultSize);
        for(unsigned int i=0; i<resultSize; i++) {
            out->result[i] = stream::readDouble(s, &cp);
        }
    }
}
