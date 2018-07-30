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
void RunCommand(args::Subparser &parser) {
    parser.Parse();
    
    state::init();
    
    Graph sine = *createSineGraph(1000);
    Graph cosine = *createCosineGraph(1000);

    analyze::create_patterns(patterns, sine);
    analyze::train(&matches, patterns, cosine);
    analyze::predict(&predictions, patterns, matches, cosine, 0, state::totalPatterns-1, 1, cosine.data.size());

    state::preserve();
    
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
}
int main(int argc, const char **argv)
{
    args::ArgumentParser p("DataAnalytics 2.0");
    args::Group commands(p, "commands");
    args::Command import(commands, "import", "import a file to the input repository", &ImportCommand);
    args::Command run(commands, "run", "execute an entire program cycle", &RunCommand);
    
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
