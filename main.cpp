#include <iostream>
#include <fstream>
#include <cmath>
#include "data_analyzer.h"
#include "dataio.h"
#include "timer.h"

using namespace std;
using namespace dataio;

Graph *testGraph;


Graph *createSineGraph(unsigned int length) {
    Graph *out = new Graph;
    vector<vector<double>> graphData;
    for(unsigned int i=0; i<length; i++) {
        graphData.push_back({(double)i, sin((i*M_PI)/180)});
    }
    out->init(2, 0, graphData);

    return out;
}
int main()
{
    timer::start();
    testGraph = new Graph;
    testGraph->init(0, 2, {{0, 1}, {1, 2}, {2, 3}});

    Graph *sine = createSineGraph(1000);

    for(int i=0; i<3000; i++) {
        ofstream sineOut("out.txt");
        for(uint16_t i=0; i<sine->data.size(); i++) {
            for(uint16_t j=0; j<sine->data[i].size(); j++) {
                sineOut << sine->data[i][j];
                if(j!=sine->data[i].size()-1) {
                    sineOut << ",";
                }
            }
            sineOut << endl;
        }
        sineOut.close();
    }

    timer::stop("Executed");

    return 0;
}
