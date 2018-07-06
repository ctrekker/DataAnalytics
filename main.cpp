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

    for(unsigned int i=0; i<predictions.size(); i++) {
        cout << predictions[i].toString() << endl;
    }

    return 0;
}
