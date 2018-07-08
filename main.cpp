#include <iostream>
#include <fstream>
#include <cmath>
#include "data_analyzer.h"
#include "dataio.h"
#include "timer.h"

using namespace std;
using namespace dataio;

Pattern patterns[PATTERN_NUMBER];
vector<Match> matches;
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
    Graph sine = *createSineGraph(1000);
    Graph cosine = *createCosineGraph(1000);

    analyze::create_patterns(patterns, sine);
    analyze::train(&matches, patterns, cosine);
    analyze::predict(&predictions, patterns, matches, cosine, 0, PATTERN_NUMBER-1, 1, cosine.data.size());


    ofstream file("out.csv");
    for(unsigned int i=0; i<cosine.data.size(); i++) {
        for(unsigned int j=0; j<cosine.data[i].size(); j++) {
            file << cosine.data[i][j];
            if(j!=cosine.data[i].size()-1) {
                file << ",";
            }
        }
        file << endl;
    }

    int pn=0;
    int pgn=0;
    for(unsigned int i=0; i<predictions.size(); i++) {
        //cout << predictions[i].toString();
        if(predictions[i].result.size()>pgn) {
            pn=i;
            pgn=predictions[i].result.size();
        }
    }

    cout << "PN:" << pn << ";PGN:" << pgn << endl;

    for(unsigned int i=0; i<predictions[pn].result.size(); i++) {
        file << (cosine.data.size()+i) << ",";
        file << predictions[pn].result[i] << endl;
    }
    //cout << predictions[pn].toString() << endl;
    file.close();

    return 0;
}
