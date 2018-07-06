#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

enum PredictionHandler {
    SEPARATE, MERGED
};

const static int PATTERN_NUMBER = 1000;
const static int PATTERN_LENGTH = 30;

const static double TRAINING_THRESHOLD = 1;
const static double TRAINING_END_THRESHOLD = 0.1;

const static int PREDICTION_HANDLER = PredictionHandler::SEPARATE;
const static bool PREDICTION_RECURSIVE = false;
const static int PREDICTION_MAX_RECURSIVE_ATTEMPTS = 3;
const static double PREDICTION_ACCEPTANCE_THRESHOLD = 1.0;

const static int DIMENSIONS = 2;

#endif // CONFIG_H_INCLUDED
