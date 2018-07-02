#include <iostream>
#include <chrono>
#include <windows.h>
#include "data_analyzer.h"

namespace analyze {
    void create_patterns() {
        std::cout << "Creating patterns..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();



        auto finish = std::chrono::high_resolution_clock::now();
        std::cout << "Finished creating patterns in " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns" << std::endl;
    }
    void train() {

    }
    void predict() {

    }
}
