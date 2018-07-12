#include <fstream>
#include "dataio.h"
#include "stream.h"
#include "save.h"

using namespace std;
using namespace dataio;

namespace save {
    void pattern(vector<uint8_t>* out, Pattern* p) {
        stream::writeLong(out, p->id);
        stream::writeShort(out, p->dimensions);
        stream::writeLong(out, p->created);
        stream::writeInt(out, p->length);
        stream::writeInt(out, p->corrections);
        stream::writeDouble(out, p->averageError);

        vector<vector<double>>* body;
        int it=0;
        do {
            if(it==0) body = &p->body;
            else body = &p->resultBody;

            for(unsigned int i=0; i<body->size(); i++) {
                for(unsigned int j=0; j<(*body)[i].size(); j++) {
                    stream::writeDouble(out, (*body)[i][j]);
                }
            }

            it++;
        } while(body!=&p->resultBody);
    }
    void matchList(vector<uint8_t>* out, MatchList* m) {

    }
    void match(vector<uint8_t>* out, Match* m) {

    }
    void prediction(vector<uint8_t>* out, Prediction* p) {

    }

    void patternList(vector<Pattern>* patterns, ofstream* outFile) {
        vector<uint8_t> bos;
        for(unsigned int i=0; i<PATTERN_NUMBER; i++) {
            pattern(&bos, &(*patterns)[i]);
            buffToFile(&bos, outFile);
            bos.clear();
        }
    }

    void buffToFile(vector<uint8_t>* buff, ofstream* outFile) {
        char cBuff[buff->size()];
        for(unsigned int i=0; i<buff->size(); i++) {
            cBuff[i] = (*buff)[i];;
        }
        outFile->write(cBuff, buff->size());
    }
}
