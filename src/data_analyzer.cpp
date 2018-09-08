#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cmath>
#include <float.h>

#include "ProgressBar.hpp"
#include "data_analyzer.h"
#include "timer.h"
#include "dataio.h"
#include "config.h"
#include "state.h"
#include "save.h"
#include "load.h"
#include "log.h"

using namespace dataio;

extern Log LOG;

namespace analyze {
    void create_patterns(vector<Pattern> &patternArr, Graph graph) {
        LOG.info("Creating patterns...");
        timer::start();

        ProgressBar pb(PATTERN_NUMBER, 50);
        uint64_t lastLoadUpdate = timer::getTimeMillis();

        for(int i=0; i<PATTERN_NUMBER; i++) {
            int startPos = rand() % (graph.data.size() - PATTERN_LENGTH*2);

            Pattern p;
            p.id = state::getPatternId(i);
            p.dimensions = graph.dimensions;
            p.created = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            p.length = PATTERN_LENGTH;
            p.corrections = 0;
            p.averageError = DBL_MAX;

            for(unsigned int x=0; x<p.length; x++) {
                p.body.push_back(graph.data[startPos+x]);
                p.resultBody.push_back(graph.data[startPos+x+p.length]);
            }
            patternArr[i] = p;

            if(timer::getTimeMillis()-lastLoadUpdate>150) {
                pb.setTicks(i);
                pb.display();
                lastLoadUpdate = timer::getTimeMillis();
            }
        }
        state::totalPatterns += PATTERN_NUMBER;
        save::createdPatterns(&patternArr);

        pb.done();
        timer::stop("Created patterns");
    }
    void train(vector<MatchList>* matchArr, vector<Pattern> patterns, Graph graph) {
       LOG.info("Training model...");
        timer::start();

        ProgressBar pb(state::totalPatterns, 50);

        int currentPatternFileId = 0;
        load::patternFile(&patterns, currentPatternFileId);
        if(state::initTotalPatterns!=0) {
            load::matchFile(matchArr, currentPatternFileId);
        }
        uint64_t lastLoadUpdate = timer::getTimeMillis();
        for(uint64_t i=0; i<state::totalPatterns; i++) {
            if(state::getFileId(i)!=currentPatternFileId) {
                // Save new matches
                save::createdMatches(matchArr, currentPatternFileId, true);
                // Assign new file id
                currentPatternFileId=state::getFileId(i);
                // Load old matches
                if(state::matchFileExists(currentPatternFileId)) {
                    load::matchFile(matchArr, currentPatternFileId);
                }
                else {
                    matchArr->clear();
                }
                // Load next patterns
                load::patternFile(&patterns, currentPatternFileId);
            }
            if(matchArr->size()<=i%OBJ_PER_FILE) {
                MatchList m = *(new MatchList);
                m.id = i;
                matchArr->push_back(m);
            }
            Pattern p = patterns[i%OBJ_PER_FILE];
            vector<Match> mList;
            patternMatch(&mList, graph, p, true);

            MatchList* m = &((*matchArr)[i%OBJ_PER_FILE]);
            for(unsigned int j=0; j<mList.size(); j++) {
                if(j>MATCH_BUFFER_SIZE) {
                    break;
                }
                m->addMatch(mList[j]);
            }
            if(timer::getTimeMillis()-lastLoadUpdate>150) {
                pb.setTicks(i);
                pb.display();
                lastLoadUpdate = timer::getTimeMillis();
            }
        }
        save::createdMatches(matchArr, currentPatternFileId, true);

        pb.done();
        timer::stop("Trained model");
    }
    bool predict(vector<Prediction>* predictions, vector<Pattern> &patterns, vector<MatchList> matches, Graph graph, uint64_t patternStart, uint64_t patternEnd, int layer, int initialGraphSize) {
        if(layer==1) {
            LOG.info("Predicting outcomes...");
            timer::start();
        }
        ProgressBar pb(patternEnd-patternStart, 50);
        uint64_t lastLoadUpdate = timer::getTimeMillis();

        bool added = false;

        int currentPatternFileId = state::getFileId(patternStart);
        load::patternFile(&patterns, currentPatternFileId);
        load::matchFile(&matches, currentPatternFileId);
        for(uint64_t pid=patternStart; pid<=patternEnd; pid++) {
            if(state::getFileId(pid)!=currentPatternFileId) {
                currentPatternFileId = state::getFileId(pid);
                load::patternFile(&patterns, currentPatternFileId);
                load::matchFile(&matches, currentPatternFileId);
            }

            Pattern p = patterns[pid%OBJ_PER_FILE];
            vector<Match> mList;
            mList = matches[pid%OBJ_PER_FILE].matches;

            vector<Match> data;
            patternMatch(&data, graph, p, false);

            double totalBellWeight = 0;
            for(uint64_t i=0; i<mList.size(); i++) {
                Match m = mList[i];

                //if(m==nullptr) continue;
                double bell = bellCurve(TRAINING_THRESHOLD, 1, m.error);
                totalBellWeight += bell;
            }

            short int matchIndex = 0;

            double lowestError = DBL_MAX;
            Match* bestMatch = nullptr;
            for(uint64_t i=0; i<mList.size(); i++) {
                Match* m = &mList[i];
                if(abs(m->error)<lowestError) {
                    lowestError = abs(m->error);
                    bestMatch = m;
                }
            }

            if(bestMatch == nullptr) {
                LOG.warning(to_string(pid) + " nomatch");
                continue;
            }
            double bellWeight = bellCurve(TRAINING_THRESHOLD, 1, bestMatch->error);
            if(bellWeight < PREDICTION_ACCEPTANCE_THRESHOLD) {
                continue;
            }
            for(uint64_t i=0; i<data.size(); i++) {
                Match matchData=data[i];
                vector<double> rawPredictedData = sequenceTranslate(graph.data[graph.data.size()-1][1], matchData.translateData(sequenceTransform(p.getResultBodyDimension(1), bestMatch->data)));
                // Remove the first entry, as it is the same point as the last known point on the input dataset
                rawPredictedData.erase(rawPredictedData.begin());
                vector<double> predictedData = combineSequence(graph.clip(initialGraphSize, graph.data.size()-initialGraphSize).getPlotArray(1),
                                                       rawPredictedData);

                Prediction prediction;
                Prediction rawPrediction;

                prediction.init(pid, matchIndex, predictedData, bellWeight, bellWeight/totalBellWeight);
                rawPrediction.init(pid, matchIndex, rawPredictedData, bellWeight, bellWeight/totalBellWeight);
                if(PREDICTION_HANDLER == PredictionHandler::MERGED||((PREDICTION_HANDLER == PredictionHandler::SEPARATE)&&(!PREDICTION_RECURSIVE||layer>=PREDICTION_MAX_RECURSIVE_ATTEMPTS))) {
                    if(!prediction.containedIn(predictions)) {
                        predictions->push_back(prediction);
                    }
                }
                else {
                    vector<vector<double>> addedData(graph.data.size()+rawPrediction.result.size(), vector<double>(graph.dimensions));
                    double diff = graph.getPlotArray(graph.timeIndex)[graph.data.size()-1]-graph.getPlotArray(graph.timeIndex)[graph.data.size()-2];
                    double lastTime = graph.getPlotArray(graph.timeIndex)[graph.data.size()-1];
                    for(unsigned int x=0; x<addedData.size(); x++) {
                        for(unsigned int y=0; y<addedData[0].size(); y++) {
                            if(x<graph.data.size()) {
                                addedData[x][y] = graph.data[x][y];
                            }
                            else {
                                if(y==0) {
                                    // Apply a linear regressive algorithm to determine predicted time value
                                    addedData[x][y] = lastTime+diff*((x+1)-graph.data.size());
                                }
                                else {
                                    // Apply the actual predicted values to the addedData vector
                                    addedData[x][y] = rawPrediction.result[x-graph.data.size()];
                                }
                            }
                        }
                    }

                    Graph addedGraph;
                    addedGraph.init(graph.dimensions, graph.timeIndex, addedData);
                    if(!predict(predictions, patterns, matches, addedGraph, patternStart, patternEnd, layer+1, initialGraphSize)) {
                        predictions->push_back(prediction);
                    }
                }

                if(predictions->size()>=PREDICTION_MAX_NUMBER) {
                    if(layer==1) {
                        pb.done();
                    }
                    return added;
                }
            }
            if(layer==1) {
                if(timer::getTimeMillis()-lastLoadUpdate>150) {
                    pb.setTicks(pid-patternStart);
                    pb.display();
                    lastLoadUpdate = timer::getTimeMillis();
                }
            }
//            if(layer<PREDICTION_MAX_RECURSIVE_ATTEMPTS) {
//                for(int gt=layer; gt>0; gt--) {
//                    cout << ">";
//                }
//                cout << "Finished " << pid << endl;
//            }
        }
        if(layer==1) {
            pb.done();
            timer::stop("Predicted outcomes");
        }
        return added;
    }
    void printVector(vector<double> d) {
        for(unsigned int i=0; i<d.size(); i++) {
            cout << d[i];
            if(d.size()-1!=i) {
                cout << ",";
            }
        }
        cout << endl;
    }
    void patternMatch(vector<Match>* matches, Graph graph, Pattern pattern, bool knownResult) {
        if(knownResult) {
            return patternMatch(matches, graph, pattern, 0, graph.data.size() - PATTERN_LENGTH * 2 + 1, true);
        }
        else {
            return patternMatch(matches, graph, pattern, graph.data.size() - PATTERN_LENGTH, graph.data.size() - PATTERN_LENGTH + 1, false);
        }
    }
    void patternMatch(vector<Match>* matches, Graph graph, Pattern pattern, int start, int end, bool doResult) {
        vector<vector<double>> pBody = pattern.body;

        for(int i = start; i < end; i++) {
            double totalError = 0;
            double translation = graph.data[i][1] - pBody[0][1];
            double g0 = graph.data[i][1];
            double p0 = pBody[0][1];
            double g1 = graph.data[i+1][1];
            double p1 = pBody[1][1];
            // Slope-intercept (distance formula kinda)
            double si = (g1 - g0) / (p1 - p0);
            double endError = DBL_MAX;
            if(p1 - p0 == 0) continue;

            vector<double> gResultMod(PATTERN_LENGTH);

            for(int j = 0; j < PATTERN_LENGTH; j++) {
                vector<double>* graphPoint = &graph.data[i + j];
                vector<double>* patternPoint = &pBody[j];
                double gv = (*graphPoint)[1];
                double pv = (*patternPoint)[1];
                double pm = (si * (pv - p0) + p0 + translation);
                if(j == PATTERN_LENGTH - 1) {
                    endError = abs(pm - gv);
                }
                totalError += abs(pm - gv);

                if(doResult) gResultMod[j] = graph.data[i + j + PATTERN_LENGTH][1];
            }
            if(totalError < TRAINING_THRESHOLD && endError < TRAINING_END_THRESHOLD) {
                Match match;
                match.data = gResultMod;
                match.slopeIntercept = si;
                match.translation = translation;
                match.patternZero = p0;
                match.error = totalError;
                match.pid = pattern.id;
                matches->push_back(match);
                if(!doResult) {

                }
            }
        }
    }

    void getMatchesByPid(vector<Match>* matchOut, vector<Match>* matches, uint64_t pid) {
        for(uint64_t i=0; i<matches->size(); i++) {
            if((*matches)[i].pid==pid) {
                matchOut->push_back((*matches)[i]);
            }
        }
    }

    double bellCurve(double val) {
        return bellCurve(1, 1, val);
    }
    double bellCurve(double width, double height, double val) {
        val /= width;
        return height * exp(pow(2*val, 2));
    }

    vector<double> sequenceTransform(vector<double> set1, vector<double> set2) {
        int length = set2.size();
        vector<double> translatedSet = *new vector<double>(length);

        double s1_0 = set1[0];
        double s2_0 = set2[0];
        double s1_1 = set1[1];
        double s2_1 = set2[1];
        double translation = s1_0 - s2_0;
        double si = (s1_1 - s1_0) / (s2_1 - s2_0);
        if(s2_1 - s2_0 == 0) si = (s1_1 - s1_0);

        for(int i=0; i<length; i++) {
            double pv = set2[i];
            double s2_mod = (si * (pv - s2_0) + s2_0 + translation);
            translatedSet[i] = s2_mod;
        }

        return translatedSet;
    }
    vector<double> sequenceTranslate(double base, vector<double> ls) {
        int listLength = ls.size();
        vector<double> out = *new vector<double>(listLength);
        double translation = base - ls[0];
        for(int i=0; i<listLength; i++) {
            out[i] = ls[i] + translation;
        }
        return out;
    }
    vector<double> combineSequence(vector<double> first, vector<double> second) {
        int firstLength = first.size();
        int secondLength = second.size();
        int outLength = firstLength + secondLength;
        vector<double> out = *new vector<double>(outLength);
        for(int i=0; i<outLength; i++) {
            if(i<firstLength) {
                out[i] = first[i];
            }
            else {
                out[i] = second[i - firstLength];
            }
        }
        return out;
    }
}
