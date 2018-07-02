#include <vector>
#include "dataio.h"
#include "config.h"

using namespace std;

namespace dataio {
    struct Graph {
        public:
            int timeIndex;
            short int dimensions;
            vector<vector<double>> data;

            void init(short int dimensions, int timeIndex, auto data) {
                this->dimensions = dimensions;
                this->timeIndex = timeIndex;
                this->data = data;
            }

    };
    struct Pattern {

    };
    struct Match {

    };
    struct MatchList {

    };
    struct Prediction {

    };
}
