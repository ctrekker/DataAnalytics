#include <iostream>
#include <fstream>
#include <cmath>

#include "data_analyzer.h"
#include "dataio.h"
#include "timer.h"
#include "stream.h"
#include "save.h"
#include "load.h"
#include "state.h"

using namespace std;
using namespace dataio;

vector<Pattern> patterns(PATTERN_NUMBER);
vector<MatchList> matches;
vector<Prediction> predictions;

Graph *createSineGraph(unsigned int length) {
    Graph *out = new Graph;
    vector<vector<double>> graphData;
    for(unsigned int i=0; i<length; i++) {
        graphData.push_back({(double)i, sin((i*M_PI)/180)});
    }
    out->init(2, 0, graphData);

    return out;
}
Graph *createCosineGraph(unsigned int length) {
    Graph *out = new Graph;
    vector<vector<double>> graphData;
    for(unsigned int i=0; i<length; i++) {
        graphData.push_back({(double)i, cos((i*M_PI)/180)});
    }
    out->init(2, 0, graphData);

    return out;
}
int main()
{
    state::init();
    
    Graph sine = *createSineGraph(1000);
    Graph cosine = *createCosineGraph(1000);

    analyze::create_patterns(patterns, sine);
    analyze::train(&matches, patterns, cosine);
    analyze::predict(&predictions, patterns, matches, cosine, 0, PATTERN_NUMBER-1, 1, cosine.data.size());


//    timer::start();
//    ofstream patternFile("data/patterns.bin", ios::binary);
//    save::patternList(&patterns, &patternFile);
//    timer::stop("Wrote patterns to disk");
//    patternFile.close();
//
//    timer::start();
//    ofstream matchFile("data/matches.bin", ios::binary);
//    save::matchListCollection(&matches, &matchFile);
//    timer::stop("Wrote matches to disk");
//    patternFile.close();
//
//    timer::start();
//    ofstream predictionFile("data/predictions.bin", ios::binary);
//    save::predictionList(&predictions, &predictionFile);
//    timer::stop("Wrote predictions to disk");
//    predictionFile.close();
    state::preserve();
    save::state("data", &patterns, &matches, &predictions);
    
    cout << endl;
    cout << "---DEBUG---" << endl;

    cout << predictions.size() << "<-Total Predictions" << endl;


    ofstream file("data/out.csv");
    for(unsigned int i=0; i<cosine.data.size(); i++) {
        for(unsigned int j=0; j<cosine.data[i].size(); j++) {
            file << cosine.data[i][j];
            if(j!=cosine.data[i].size()-1) {
                file << ",";
            }
        }
        file << endl;
    }

    unsigned int pn=0;
    unsigned int pgn=0;
    for(unsigned int i=0; i<predictions.size(); i++) {
        if(predictions[i].result.size()>pgn) {
            pn=i;
            pgn=predictions[i].result.size();
        }
    }

    cout << "PN:" << pn << ";PGN:" << pgn << endl;

    if(pgn>0) {
        for(unsigned int i=0; i<predictions[pn].result.size(); i++) {
            file << (cosine.data.size()+i) << ",";
            file << predictions[pn].result[i] << endl;
        }
        cout << predictions[pn].toString() << endl;
    }
    file.close();
    
    return 0;
}
