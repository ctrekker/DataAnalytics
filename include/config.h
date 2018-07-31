#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

using namespace std;

enum PredictionHandler {
    SEPARATE, MERGED
};

const static string SAVE_DIR = "save";
const static string ARCH_DIR = "arch";

const static uint64_t OBJ_PER_FILE = 2000;

const static string INPUT_REPO_LOCATION = "in";

const static int PATTERN_NUMBER = 1000;
const static int PATTERN_LENGTH = 30;

const static double TRAINING_THRESHOLD = 0.01;
const static double TRAINING_END_THRESHOLD = 0.001;

const static short int MATCH_BUFFER_SIZE = 25;
const static int MATCH_MAX_DATA_SIZE = PATTERN_LENGTH;

const static int PREDICTION_HANDLER = PredictionHandler::SEPARATE;
const static int PREDICTION_MAX_NUMBER = 1000;
const static bool PREDICTION_RECURSIVE = false;
const static int PREDICTION_MAX_RECURSIVE_ATTEMPTS = 3;
const static double PREDICTION_ACCEPTANCE_THRESHOLD = 1.0;

const static int DIMENSIONS = 2;

#endif // CONFIG_H_INCLUDED
