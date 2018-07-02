#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cmath>
#include <float.h>
#include "data_analyzer.h"
#include "timer.h"
#include "dataio.h"
#include "config.h"

using namespace dataio;

namespace analyze {
    void create_patterns(Pattern *patternArr, Graph graph) {
        std::cout << "Creating patterns..." << std::endl;
        timer::start();
        
        for(int i=0; i<PATTERN_NUMBER; i++) {
            int startPos = rand() % (graph.data.size() - PATTERN_LENGTH*2);
            
            Pattern p;
            p.id = i;
            p.dimensions = graph.dimensions;
            p.created = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            p.length = PATTERN_LENGTH;
            p.corrections = 0;
            p.averageError = DBL_MAX;
            
            for(unsigned int x=0; x<p.length; x++) {
                p.body[x] = graph.data[startPos+x];
                p.resultBody[x] = graph.data[startPos+x+p.length];
            }
            patternArr[i] = p;
        }
        
        timer::stop("Created patterns");
    }
    void train(vector<Match> matchArr, Graph graph, Pattern* patterns) {
        std::cout << "Training model..." << std::endl;
        timer::start();
        
        for(int i=0; i<PATTERN_NUMBER; i++) {
            Pattern p = patterns[i];
            vector<Match> matchList;
            patternMatch(&matchList, graph, p, true);
        }
        
        timer::stop("Trained model");
    }
    void predict() {

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
        vector<double>* pBody = pattern.body;
        
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
            if(isnan(si)) si = 1;
            
            double gResultMod[PATTERN_LENGTH];
            
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
                copy(gResultMod, gResultMod+PATTERN_LENGTH, match.data);
                match.slopeIntercept = si;
                match.translation = translation;
                match.patternZero = p0;
                match.error = totalError;
                matches->push_back(match);
                if(!doResult) {
                    // Not implemented
                }
            }
        }
    }
}
