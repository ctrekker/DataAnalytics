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
    analyze::predict(&predictions, patterns, matches, cosine, 0, state::totalPatterns-1, 1, cosine.data.size());

    cout << "---DEBUG---" << endl;
    state::preserve();
    cout << endl;

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
