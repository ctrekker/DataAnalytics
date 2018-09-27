#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include "log.h"

using namespace std;

extern Log LOG;

enum PredictionHandler {
    SEPARATE, MERGED
};

// Note that the actual prediction output is 1 value less than the pattern length
const static unsigned int PATTERN_LENGTH = 41;

const static short int MATCH_BUFFER_SIZE = 40;
// This value should ALWAYS be equal to PATTERN_LENGTH
const static int MATCH_MAX_DATA_SIZE = 41;
const static int DIMENSIONS = 2;

class Config {
public:
    string SAVE_DIR = "save";
    string SAVE_DIR_DEFAULT = "save";

    string ARCH_DIR = "arch";
    string OUT_DIR = "data";

    uint64_t OBJ_PER_FILE = 50000;

    string INPUT_REPO_LOCATION = "in";
    string EXECUTION_NAME = "latest";
    bool SHOW_PROGRESS_BARS = true;

    int PATTERN_NUMBER = 500;

    double TRAINING_THRESHOLD = 0.01;
    double TRAINING_END_THRESHOLD = 0.001;

    int PREDICTION_HANDLER = PredictionHandler::SEPARATE;
    unsigned int PREDICTION_MAX_NUMBER = 1000;
    bool PREDICTION_RECURSIVE = false;
    int PREDICTION_MAX_RECURSIVE_ATTEMPTS = 3;
    double PREDICTION_ACCEPTANCE_THRESHOLD = 1.0;

    void print(bool pretty) {
        if(pretty) {
            LOG.debug("Save Dir Default: " + SAVE_DIR_DEFAULT);
            LOG.debug("Out Dir: " + OUT_DIR);
            LOG.debug("Objects per File: " + to_string(OBJ_PER_FILE));
            LOG.debug("Input Repository Location: " + INPUT_REPO_LOCATION);
            LOG.debug("Show Progress Bars: " + string(SHOW_PROGRESS_BARS?"true":"false"));
            LOG.debug("Pattern Length: " + to_string(PATTERN_LENGTH));
            LOG.debug("Pattern Number: " + to_string(PATTERN_NUMBER));
            LOG.debug("Training Threshold: " + to_string(TRAINING_THRESHOLD));
            LOG.debug("Training End Threshold: " + to_string(TRAINING_END_THRESHOLD));
            LOG.debug("Prediction Max Number: " + to_string(PREDICTION_MAX_NUMBER));
            LOG.debug("Prediction Recursive: " + string(PREDICTION_RECURSIVE?"true":"false"));
            LOG.debug("Prediction Max Recursive Attempts: " + to_string(PREDICTION_MAX_RECURSIVE_ATTEMPTS));
            LOG.debug("Prediction Acceptance Threshold: " + to_string(PREDICTION_ACCEPTANCE_THRESHOLD));
            LOG.debug("Match Buffer Size: " + to_string(MATCH_BUFFER_SIZE));
            LOG.debug("Match Max Data Size: " + to_string(MATCH_MAX_DATA_SIZE));
        }
        else {
            LOG.debug("Config:");
            LOG.debug("\tSAVE_DIR_DEFAULT                  = " + SAVE_DIR_DEFAULT);
            LOG.debug("\tOUT_DIR                           = " + OUT_DIR);
            LOG.debug("\tOBJ_PER_FILE                      = " + to_string(OBJ_PER_FILE));
            LOG.debug("\tINPUT_REPO_LOCATION               = " + INPUT_REPO_LOCATION);
            LOG.debug("\tSHOW_PROGRESS_BARS                = " + string(SHOW_PROGRESS_BARS?"true":"false"));
            LOG.debug("\tPATTERN_LENGTH                    = " + to_string(PATTERN_LENGTH));
            LOG.debug("\tPATTERN_NUMBER                    = " + to_string(PATTERN_NUMBER));
            LOG.debug("\tTRAINING_THRESHOLD                = " + to_string(TRAINING_THRESHOLD));
            LOG.debug("\tTRAINING_END_THRESHOLD            = " + to_string(TRAINING_END_THRESHOLD));
            LOG.debug("\tPREDICTION_MAX_NUMBER             = " + to_string(PREDICTION_MAX_NUMBER));
            LOG.debug("\tPREDICTION_RECURSIVE              = " + string(PREDICTION_RECURSIVE?"true":"false"));
            LOG.debug("\tPREDICTION_MAX_RECURSIVE_ATTEMPTS = " + to_string(PREDICTION_MAX_RECURSIVE_ATTEMPTS));
            LOG.debug("\tPREDICTION_ACCEPTANCE_THRESHOLD   = " + to_string(PREDICTION_ACCEPTANCE_THRESHOLD));
            LOG.debug("\tMATCH_BUFFER_SIZE                 = " + to_string(MATCH_BUFFER_SIZE));
            LOG.debug("\tMATCH_MAX_DATA_SIZE               = " + to_string(MATCH_MAX_DATA_SIZE));
        }
    }
};


#endif // CONFIG_H_INCLUDED
