#ifdef _WIN32
   // Windows (32-bit and 64-bit)
   #define OS "WIN"
   #define SEPERATOR "\\"
   #define INIT_SCRIPT "\\dir.bat"
#elif __APPLE__
    // Apple OS
    #define INIT_SCRIPT "/dir.sh"
    #define SEPERATOR "/"
#elif __linux
    // linux
    #define INIT_SCRIPT "/dir.sh"
    #define SEPERATOR "/"
#elif __unix // all unices not caught above
    // Unix
    #define INIT_SCRIPT "/dir.sh"
    #define SEPERATOR "/"
#elif __posix
    // POSIX
    #define INIT_SCRIPT "DNE"
#endif

#ifndef OS
    #define OS "OTHER"
#endif // OS

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
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
void runScript(string name) {
    string scriptPath;
    if(strcmp(name.c_str(), "DNE") == 0) {
        cout << "Unsupported OS: POSIX";
        exit(EXIT_FAILURE);
    }
    else {
        if(strcmp(OS, "WIN") != 0) {
            scriptPath += "./";
        }
        scriptPath += "sh" + string(SEPERATOR) + string(name);
    }

    system(scriptPath.c_str());
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
    args::ValueFlag<string> nameFlag(parser, "NAME", "name of the current execution. Defaults to \"latest\"", {'n', "name"});
    args::Flag patternFlag(parser, "PATTERN", "create patterns or not", {'p'});
    args::Flag trainFlag(parser, "TRAIN", "train model or not", {'t'});
    args::Flag predictFlag(parser, "PREDICT", "make predictions for source", {'r'});
    args::Flag verboseFlag(parser, "VERBOSE", "turn on verbose output - UNIMPLEMENTED", {'v', "verbose"});
    args::Flag debugFlag(parser, "DEBUG", "turn on debug mode - UNIMPLEMENTED", {'d', "debug"});
    parser.Parse();

    state::init();

    // Default is defined in config.h
    if(nameFlag) {
        EXECUTION_NAME = args::get(nameFlag);
    }

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


    ofstream file(OUT_DIR+"/"+EXECUTION_NAME+".csv");
    save::csvPredictionList(&graph, &predictions, file);
    file.close();

    unsigned int pn=0;
    unsigned int pgn=0;
    for(unsigned int i=0; i<predictions.size(); i++) {
        if(predictions[i].result.size()>pgn) {
            pn=i;
            pgn=predictions[i].result.size();
        }
    }

    cout << "PN:" << pn << ";PGN:" << pgn << endl;

    cout << predictions[pn].result.size();
    if(pgn>0) {
        for(unsigned int i=0; i<predictions[pn].result.size(); i++) {
            file << (graph.data.size()+i) << ",";
            file << predictions[pn].result[i] << endl;
        }
        cout << predictions[pn].toString() << endl;
    }
}
void ExportCommand(args::Subparser &parser) {
    args::ValueFlag<string> outputFlag(parser, "OUTPUT", "a file path to output selected data to", {'o', "output"});
    args::ValueFlag<string> nameFlag(parser, "NAME", "name of the designated run to output from", {'n', "name"});
    parser.Parse();

    string outputPath = "out.csv";
    if(outputFlag) {
        outputPath = args::get(outputFlag);
    }
    string name = "latest";
    if(nameFlag) {
        name = args::get(nameFlag);
    }
    string runPath = OUT_DIR+"/"+name+".csv";


    cout << "Exporting " << name << " to " << outputPath << endl;
    timer::start();
    // The actual output the user expects
    ofstream outputFile(outputPath);
    // The output from the algorithm run
    ifstream runFile(runPath);

    string line;
    while(getline(runFile, line)) {
        outputFile << line << endl;
    }

    outputFile.close();
    runFile.close();

    timer::stop("Finished export");
}
void CleanCommand(args::Subparser &parser) {
    args::Flag hardFlag(parser, "HARD", "resets everything as opposed to just caches", {'h', "hard"});
    parser.Parse();

    cout << "Executing cleaning script" << endl;

    timer::start();
    runScript("clean");
    if(hardFlag) {
        runScript("cleanhard");
    }
    cout << endl << endl;
    timer::stop("Finished cleaning");
}
void InfoCommand(args::Subparser &parser) {
    args::Flag debugFlag(parser, "DEBUG", "shows debug output rather than clean value-pair output", {'d', "debug"});
    parser.Parse();

    state::init(false);
    state::print(!debugFlag);
}
void InitCommand(args::Subparser &parser) {
    parser.Parse();

    string scriptPath;
    // Avoid a warning with strcmp


    cout << "Executing OS-specific executable: " << scriptPath << endl;

    timer::start();
    runScript("dir");
    cout << endl << endl;
    timer::stop("Finished init");
}
int main(int argc, const char **argv)
{
    args::ArgumentParser p("DataAnalytics 2.0");
    args::Group commands(p, "commands");
    args::Command import(commands, "import", "import a file to the input repository", &ImportCommand);
    args::Command run(commands, "run", "execute an entire program cycle", &RunCommand);
    args::Command exportCmd(commands, "export", "export the last or a specified run's output to a usable format and external location", &ExportCommand);
    args::Command cleanCmd(commands, "clean", "cleans out generated folders and resets configurations", &CleanCommand);
    args::Command info(commands, "info", "view information about current run state", &InfoCommand);
    args::Command init(commands, "init", "initialize the directory structure with the required paths", &InitCommand);

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
