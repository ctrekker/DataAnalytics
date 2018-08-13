#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>

#include "data_analyzer.h"
#include "dataio.h"
#include "timer.h"
#include "stream.h"
#include "save.h"
#include "load.h"
#include "state.h"
#include "config.h"
#include "args.hxx"

using namespace std;
using namespace dataio;

vector<Pattern> patterns(PATTERN_NUMBER);
vector<MatchList> matches;
vector<Prediction> predictions;

inline bool file_exists(string name) {
    ifstream file(name.c_str());
    return file.good();
}
vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}
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
void ImportCommand(args::Subparser &parser) {
    args::ValueFlag<string> pathFlag(parser, "PATH", "file path or database path to desired import resource", {'p', "path"});
    args::ValueFlag<string> typeFlag(parser, "TYPE", "type of data source to import from", {'t', "type"});
    args::ValueFlag<string> locationFlag(parser, "LOCATION", "location of the repository", {'l'});
    args::ValueFlag<string> nameFlag(parser, "NAME", "name of imported data source", {'n', "name"});
    args::Flag viewFlag(parser, "VIEW", "view a list of all imported resources", {'v', "view"});
    parser.Parse();

    if(!pathFlag&&!viewFlag) {
        cout << "ERROR: a path is required";
    }
    else if(viewFlag) {
        string location = INPUT_REPO_LOCATION;
        if(locationFlag) {
            location = args::get(locationFlag);
        }

        cout << "View is not yet supported" << endl;
    }
    else {
        string path = args::get(pathFlag);
        vector<string> pathSplit = split(path, '/');
        string type = "csv";
        string location = INPUT_REPO_LOCATION;
        string name = pathSplit[pathSplit.size()-1];

        if(typeFlag) {
            type = args::get(typeFlag);
        }
        if(locationFlag) {
            location = args::get(locationFlag);
        }
        if(nameFlag) {
            name = args::get(nameFlag);
        }

        if(type=="csv") {
            ofstream repoOut(location+"/"+name);
            ifstream repoIn(path);
            cout << "Importing " << path << " to " << location+"/"+name << endl;
            string line;
            while(getline(repoIn, line)) {
                repoOut << line << endl;
            }
            repoOut.close();
            repoIn.close();
        }
    }
}
void RunCommand(args::Subparser &parser) {
    args::ValueFlag<string> sourceFlag(parser, "SOURCE", "source of the resource to run from. Defaults to test case", {'s', "source"});
    args::Flag patternFlag(parser, "PATTERN", "create patterns or not", {'p'});
    args::Flag trainFlag(parser, "TRAIN", "train model or not", {'t'});
    args::Flag predictFlag(parser, "PREDICT", "make predictions for source", {'r'});
    args::Flag verboseFlag(parser, "VERBOSE", "turn on verbose output", {'v', "verbose"});
    args::Flag debugFlag(parser, "DEBUG", "turn on debug mode", {'d', "debug"});
    parser.Parse();

    state::init();

    // Check for custom graph or a test source
    Graph graph;
    if(sourceFlag) {
        ifstream repoIn(INPUT_REPO_LOCATION+"/"+args::get(sourceFlag));
        string line;
        vector<vector<double>> graphData;
        while(getline(repoIn, line)) {
            vector<string> csvSplit = split(line, ',');
            if(csvSplit.size()!=2) {
                cout << "WARNING: Malformed input file" << endl;
            }
            graphData.push_back({atof(csvSplit[0].c_str()), atof(csvSplit[1].c_str())});
        }
        graph.init(2, 0, graphData);
    }
    else {
        cout << "WARNING: Using generated test case: SINE" << endl;
        graph = *createSineGraph(1000);
    }

    if(patternFlag) {
        analyze::create_patterns(patterns, graph);
    }
    else {
        cout << "Skipping patterns" << endl;
    }
    if(trainFlag) {
        analyze::train(&matches, patterns, graph);
    }
    else {
        cout << "Skipping training" << endl;
    }
    if(predictFlag) {
        analyze::predict(&predictions, patterns, matches, graph, 0, state::totalPatterns-1, 1, graph.data.size());
    }
    else {
        cout << "Skipping predictions" << endl;
    }

    state::preserve();

    cout << endl;
    cout << "---DEBUG---" << endl;

    cout << predictions.size() << "<-Total Predictions" << endl;


    ofstream file("data/out.csv");
    for(unsigned int i=0; i<graph.data.size(); i++) {
        for(unsigned int j=0; j<graph.data[i].size(); j++) {
            file << graph.data[i][j];
            if(j!=graph.data[i].size()-1) {
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
            file << (graph.data.size()+i) << ",";
            file << predictions[pn].result[i] << endl;
        }
        cout << predictions[pn].toString() << endl;
    }
    file.close();
}
void InfoCommand(args::Subparser &parser) {
    args::Flag debugFlag(parser, "DEBUG", "shows debug output rather than clean value-pair output", {'d', "debug"});
    parser.Parse();

    state::init(false);
    state::print(!debugFlag);
}
int main(int argc, const char **argv)
{
    args::ArgumentParser p("DataAnalytics 2.0");
    args::Group commands(p, "commands");
    args::Command import(commands, "import", "import a file to the input repository", &ImportCommand);
    args::Command run(commands, "run", "execute an entire program cycle", &RunCommand);
    args::Command info(commands, "info", "view information about current run state", &InfoCommand);

    try {
        p.ParseCLI(argc, argv);
    }
    catch(args::Help) {
        std::cout << p;
    }
    catch(args::Error& e) {
        std::cerr << e.what() << std::endl << p;
        return 1;
    }

    return 0;
}
