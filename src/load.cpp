#include <vector>
#include <iostream>
#include <fstream>
#include "dataio.h"
#include "stream.h"
#include "timer.h"
#include "config.h"
#include "load.h"

using namespace std;
using namespace dataio;

namespace load {
    void patternFile(vector<Pattern>* patterns, uint64_t fid) {
        patterns->clear();
        ifstream fileIn(SAVE_DIR+"/"+to_string(fid)+".pbin");
        load::patternList(&fileIn, patterns);
        fileIn.close();
    }
    void matchFile(vector<MatchList>* matches, uint64_t fid) {
        matches->clear();
        ifstream fileIn(SAVE_DIR+"/"+to_string(fid)+".mbin");
        load::matchListCollection(&fileIn, matches);
        fileIn.close();
    }
    
    void state(string name, vector<Pattern>* patterns, vector<MatchList>* matches, vector<Prediction>* predictions) {
        cout << "Loading state..." << endl;
        timer::start();
        
        ifstream patternFile(SAVE_DIR+"/"+name+".pbin", ios::binary);
        ifstream matchFile(SAVE_DIR+"/"+name+".mbin", ios::binary);
        ifstream predictionFile(SAVE_DIR+"/"+name+".prbin", ios::binary);
        
        load::patternList(&patternFile, patterns);
        load::matchListCollection(&matchFile, matches);
        load::predictionList(&predictionFile, predictions);
        
        patternFile.close();
        matchFile.close();
        predictionFile.close();
        
        timer::stop("Loaded state");
    }
    
    int pattern(Pattern* out, vector<uint8_t>* s, uint64_t cp) {
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
        return cp;
    }
    int match(Match* out, vector<uint8_t>* s, uint64_t cp) {
        out->length = stream::readShort(s, &cp);
        out->error = stream::readDouble(s, &cp);
        out->slopeIntercept = stream::readDouble(s, &cp);
        out->translation = stream::readDouble(s, &cp);
        out->patternZero = stream::readDouble(s, &cp);
        for(unsigned int i=0; i<MATCH_MAX_DATA_SIZE; i++) {
            out->data[i] = stream::readDouble(s, &cp);
        }
        return cp;
    }
    int matchList(MatchList* out, vector<uint8_t>* s, uint64_t cp) {
        out->id = stream::readLong(s, &cp);
        out->currentIndex = stream::readShort(s, &cp);
        out->totalMatches = stream::readInt(s, &cp);
        for(unsigned int i=0; i<MATCH_BUFFER_SIZE; i++) {
            Match m;
            cp = load::match(&m, s, cp);
            out->addMatch(m, true);
        }
        return cp;
    }
    int prediction(Prediction* out, vector<uint8_t>* s, uint64_t cp) {
        out->patternId = stream::readLong(s, &cp);
        out->matchIndex = stream::readShort(s, &cp);
        uint32_t resultSize = stream::readInt(s, &cp);
        out->bellWeight = stream::readDouble(s, &cp);
        out->patternPercentage = stream::readDouble(s, &cp);

        out->result = vector<double>(resultSize);
        for(unsigned int i=0; i<resultSize; i++) {
            out->result[i] = stream::readDouble(s, &cp);
        }
        return cp;
    }
    
    void patternList(ifstream* inFile, vector<Pattern>* patterns) {
        int i=0;
        while(!inFile->eof()) {
            cout << i << endl;
            Pattern* p = new Pattern;
            vector<uint8_t> bos(Pattern::SIZE);
            load::fileToBuff(inFile, &bos);
            load::pattern(p, &bos);
            patterns->push_back(*p);
            i++;
        }
        patterns->pop_back();
    }
    void matchListCollection(ifstream* inFile, vector<MatchList>* mList) {
        int i=0;
        while(!inFile->eof()) {
            cout << i << endl;
            MatchList* ml = new MatchList;
            vector<uint8_t> bos(MatchList::SIZE);
            load::fileToBuff(inFile, &bos);
            load::matchList(ml, &bos);
            mList->push_back(*ml);
            i++;
        }
        mList->pop_back();
    }
    void predictionList(ifstream* inFile, vector<Prediction>* predictions) {
        
    }
    
    void fileToBuff(ifstream* inFile, vector<uint8_t>* buff) {
        char rawBuff[buff->size()];
        inFile->read(rawBuff, buff->size());
        for(unsigned int i=0; i<buff->size(); i++) {
            (*buff)[i] = rawBuff[i];
        }
    }
}
