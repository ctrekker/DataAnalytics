#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

using namespace std;

enum PredictionHandler {
    SEPARATE, MERGED
};

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
};

// Note that the actual prediction output is 1 value less than the pattern length
const static unsigned int PATTERN_LENGTH = 41;

const static short int MATCH_BUFFER_SIZE = 40;
// This value should ALWAYS be equal to PATTERN_LENGTH
const static int MATCH_MAX_DATA_SIZE = 41;
const static int DIMENSIONS = 2;

#endif // CONFIG_H_INCLUDED
