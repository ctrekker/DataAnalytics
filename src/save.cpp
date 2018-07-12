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
        stream::writeLong(out, m->id);
        stream::writeShort(out, m->currentIndex);
        stream::writeInt(out, m->totalMatches);
        for(unsigned int i=0; i<m->matches.size(); i++) {
            Match match = m->matches[i];
            save::match(out, &match);
        }
    }
    void match(vector<uint8_t>* out, Match* m) {
        stream::writeShort(out, m->length);
        stream::writeDouble(out, m->error);
        for(unsigned int i=0; i<MATCH_MAX_DATA_SIZE; i++) {
            if(i<m->data.size()) {
                stream::writeDouble(out, m->data[i]);
            }
            else {
                stream::writeDouble(out, 0);
            }
        }
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
    void matchListCollection(vector<MatchList>* mList, ofstream* outFile) {
        vector<uint8_t> bos;
        for(unsigned int i=0; i<mList->size(); i++) {
            save::matchList(&bos, &(*mList)[i]);
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
