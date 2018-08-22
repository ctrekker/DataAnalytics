#ifndef PROGRESSBAR_PROGRESSBAR_HPP
#define PROGRESSBAR_PROGRESSBAR_HPP

#include <chrono>
#include <iostream>
#include <sstream>

class ProgressBar {
private:
    unsigned int ticks = 0;

    const unsigned int total_ticks;
    const unsigned int bar_width;
    const char complete_char = '=';
    const char incomplete_char = ' ';
    const std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

public:
    ProgressBar(unsigned int total, unsigned int width, char complete, char incomplete) :
            total_ticks {total}, bar_width {width}, complete_char {complete}, incomplete_char {incomplete} {}

    ProgressBar(unsigned int total, unsigned int width) : total_ticks {total}, bar_width {width} {}

    unsigned int operator++() { return ++ticks; }
    void setTicks(int newTicks) {
        ticks = newTicks;
    }

    void display() const
    {
        float progress = (float) ticks / total_ticks;
        unsigned int pos = (int) (bar_width * progress);

        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now-start_time).count();

        std::stringstream stream;
        stream << "[";

        for (unsigned int i = 0; i < bar_width; ++i) {
            if (i < pos) stream << complete_char;
            else if (i == pos) stream << ">";
            else stream << incomplete_char;
        }
        stream << "] " << int(progress * 100.0) << "% "
                  << float(time_elapsed) / 1000.0 << "s\r";
        stream.flush();

        std::cout << stream.str();
    }

    void done()
    {
        ticks = total_ticks;
        display();
        std::string barClear = "";
        while(barClear.length() < this->bar_width*2) {
            barClear += "  ";
        }
        std::cout << barClear << "\r";
    }
};

#endif //PROGRESSBAR_PROGRESSBAR_HPP
