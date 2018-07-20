#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED



using namespace std;

enum PredictionHandler {
    SEPARATE, MERGED
};

const static string SWAP_DIR = "swap";
const static string SAVE_DIR = "save";
const static string ARCH_DIR = "arch";

const static int PATTERN_NUMBER = 1000000;
const static int PATTERN_LENGTH = 30;
const static int PATTERN_SWAP_THRESHOLD = 100000;

const static double TRAINING_THRESHOLD = 0.1;
const static double TRAINING_END_THRESHOLD = 0.01;

const static short int MATCH_BUFFER_SIZE = 25;
const static int MATCH_MAX_DATA_SIZE = PATTERN_LENGTH;

const static int PREDICTION_HANDLER = PredictionHandler::SEPARATE;
const static bool PREDICTION_RECURSIVE = false;
const static int PREDICTION_MAX_RECURSIVE_ATTEMPTS = 3;
const static double PREDICTION_ACCEPTANCE_THRESHOLD = 1.0;

const static int DIMENSIONS = 2;

#endif // CONFIG_H_INCLUDED
