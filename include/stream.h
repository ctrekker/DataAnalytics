#ifndef STREAM_H_INCLUDED
#define STREAM_H_INCLUDED

#include <vector>

using namespace std;

namespace stream {
    void writeLong(vector<uint8_t>*, uint64_t);
    void writeInt(vector<uint8_t>*, uint32_t);
    void writeShort(vector<uint8_t>*, uint16_t);
    void writeDouble(vector<uint8_t>*, double);
}

#endif // STREAM_H_INCLUDED
