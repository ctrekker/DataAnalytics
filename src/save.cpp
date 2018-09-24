#include <fstream>
#include <iostream>
#include <climits>
#include "dataio.h"
#include "stream.h"
#include "save.h"
#include "config.h"
#include "timer.h"
#include "state.h"
#include "log.h"

using namespace std;
using namespace dataio;

extern Log LOG;
extern Config C;

namespace save {
    void createdPatterns(vector<Pattern>* patterns) {
        ofstream outFile(state::getFilePath(state::totalPatterns-1), ios::binary | ios::app);

        save::patternList(patterns, &outFile);

        outFile.close();
    }
    void createdMatches(vector<MatchList>* matches, int fileId, bool overwrite) {
        ofstream outFile;
        if(overwrite) {
            outFile = ofstream(C.SAVE_DIR+"/"+to_string(fileId)+".mbin", ios::binary);
        }
        else {
            outFile = ofstream(C.SAVE_DIR+"/"+to_string(fileId)+".mbin", ios::binary | ios::app);
        }

        save::matchListCollection(matches, &outFile);

        outFile.close();
    }

    void state(string name, vector<Pattern>* patterns, vector<MatchList>* matches, vector<Prediction>* predictions) {
        cout << "Saving state..." << endl;
        timer::start();

        ofstream patternFile(C.SAVE_DIR+"/"+name+".pbin", ios::binary);
        ofstream matchFile(C.SAVE_DIR+"/"+name+".mbin", ios::binary);
        ofstream predictionFile(C.SAVE_DIR+"/"+name+".prbin", ios::binary);

        save::patternList(patterns, &patternFile);
        save::matchListCollection(matches, &matchFile);
        save::predictionList(predictions, &predictionFile);

        patternFile.close();
        matchFile.close();
        predictionFile.close();


        // Write metadata
        timer::stop("Saved state");
    }

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
        for(unsigned int i=0; i<MATCH_BUFFER_SIZE; i++) {
            if(i >= m->matches.size()) {
                save::match(out, new Match);
                continue;
            }
            Match match = m->matches[i];
            save::match(out, &match);
        }
    }
    void match(vector<uint8_t>* out, Match* m) {
        stream::writeShort(out, m->length);
        stream::writeDouble(out, m->error);
        stream::writeDouble(out, m->slopeIntercept);
        stream::writeDouble(out, m->translation);
        stream::writeDouble(out, m->patternZero);
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
        stream::writeLong(out, p->patternId);
        stream::writeShort(out, p->matchIndex);
        stream::writeInt(out, p->result.size());
        stream::writeDouble(out, p->bellWeight);
        stream::writeDouble(out, p->patternPercentage);
        for(unsigned int i=0; i<p->result.size(); i++) {
            stream::writeDouble(out, p->result[i]);
        }
    }

    void patternList(vector<Pattern>* patterns, ofstream* outFile) {
        vector<uint8_t> bos;
        for(unsigned int i=0; i<C.PATTERN_NUMBER; i++) {
            pattern(&bos, &(*patterns)[i]);
            if(i%64==0||i==C.PATTERN_NUMBER-1) {
                buffToFile(&bos, outFile);
                bos.clear();
            }
        }
    }
    void matchListCollection(vector<MatchList>* mList, ofstream* outFile) {
        vector<uint8_t> bos;
        int baseline=0;
        for(unsigned int i=0; i<mList->size(); i++) {
            save::matchList(&bos, &(*mList)[i]);
            if(i==0) {
                baseline=((SHRT_MAX*2)/mList->size());
            }
            if(i%baseline==0||i==mList->size()-1) {
                buffToFile(&bos, outFile);
                bos.clear();
            }
        }
    }
    void predictionList(vector<Prediction>* predictions, ofstream* outFile) {
        vector<uint8_t> bos;
        for(unsigned int i=0; i<predictions->size(); i++) {
            save::prediction(&bos, &(*predictions)[i]);
            buffToFile(&bos, outFile);
            bos.clear();
        }
    }
    void csvPredictionList(Graph* graph, vector<Prediction>* predictions, ofstream &outFile) {
        vector<double> plotArr = graph->getPlotArray(graph->timeIndex);
        vector<double> plotArrData = graph->getPlotArray(1);
        for(int i=-1; i<int(plotArr.size()); i++) {
            if(i==-1) {
                outFile << ",";
                for(unsigned int j=0; j<predictions->size(); j++) {
                    outFile << "P" << j;
                    if(j!=predictions->size()-1) {
                        outFile << ",";
                    }
                }
            }
            else {
                outFile << plotArr[i] << ",";
                for(unsigned int j=0; j<predictions->size(); j++) {
                    outFile << plotArrData[i];
                    if(j!=predictions->size()-1) {
                        outFile << ",";
                    }
                }
            }

            outFile << endl;
        }
        unsigned int maxPredLength = 0;
        if(!C.PREDICTION_RECURSIVE) {
            maxPredLength = PATTERN_LENGTH - 1;
        }
        else {
            maxPredLength = (PATTERN_LENGTH - 1) * C.PREDICTION_MAX_RECURSIVE_ATTEMPTS;
        }
        LOG.debug(to_string(maxPredLength));
        for(unsigned int i=0; i<maxPredLength; i++) {
            outFile << plotArr.size()+i << ",";
            for(unsigned int j=0; j<predictions->size(); j++) {
                if((*predictions)[j].result.size()>i+1) {
                    outFile << (*predictions)[j].result[i];
                }
                if(j!=predictions->size()-1) {
                    outFile << ",";
                }
            }
            outFile << endl;
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
