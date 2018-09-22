#ifndef STATS_H_INCLUDED
#define STATS_H_INCLUDED

using namespace std;

class Stats {
public:
    Stats();
    ~Stats();

    void setPatternTime(uint64_t time);
    void setTrainingTime(uint64_t time);
    void setPredictionTime(uint64_t time);
    void setRunTime(uint64_t time);

    void save();
private:
    ofstream patternStatsFile;
    ofstream trainingStatsFile;
    ofstream predictionStatsFile;
    ofstream runStatsFile;

    bool hasPatternTime = false;
    bool hasTrainingTime = false;
    bool hasPredictionTime = false;
    bool hasRunTime = false;

    uint64_t patternTime = 0;
    uint64_t trainingTime = 0;
    uint64_t predictionTime = 0;
    uint64_t runTime = 0;

    string toCsvRow(uint32_t runId, uint64_t time);
};

#endif // STATS_H_INCLUDED
