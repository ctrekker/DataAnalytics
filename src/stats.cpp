#include <iostream>
#include <fstream>

#include "stats.h"
#include "log.h"
#include "state.h"

using namespace std;

extern Log LOG;

Stats::Stats() {
    this->patternStatsFile    = ofstream("stats/pattern.csv",    ios::app);
    this->trainingStatsFile   = ofstream("stats/training.csv",   ios::app);
    this->predictionStatsFile = ofstream("stats/prediction.csv", ios::app);
    this->runStatsFile        = ofstream("stats/run.csv",        ios::app);
}
Stats::~Stats() {
    this->patternStatsFile.close();
    this->trainingStatsFile.close();
    this->predictionStatsFile.close();
    this->runStatsFile.close();
}

void Stats::setPatternTime(uint64_t time) {
    this->patternTime = time;
    this->hasPatternTime = time!=0;
}
void Stats::setTrainingTime(uint64_t time) {
    this->trainingTime = time;
    this->hasTrainingTime = time!=0;
}
void Stats::setPredictionTime(uint64_t time) {
    this->predictionTime = time;
    this->hasPredictionTime = time!=0;
}
void Stats::setRunTime(uint64_t time) {
    this->runTime = time;
    this->hasRunTime = time!=0;
}

void Stats::save() {
    if(this->hasPatternTime) {
        this->patternStatsFile << this->toCsvRow(state::runId, this->patternTime);
    }
    if(this->hasTrainingTime) {
        this->trainingStatsFile << this->toCsvRow(state::runId, this->trainingTime);
    }
    if(this->hasPredictionTime) {
        this->predictionStatsFile << this->toCsvRow(state::runId, this->predictionTime);
    }
    if(this->hasRunTime) {
        this->runStatsFile << this->toCsvRow(state::runId, this->runTime);
    }
}

string Stats::toCsvRow(uint32_t runId, uint64_t time) {
    return to_string(runId) + "," + to_string(double(time)/1000.0) + "\n";
}
