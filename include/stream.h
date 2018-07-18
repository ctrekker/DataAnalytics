#ifndef STREAM_H_INCLUDED
#define STREAM_H_INCLUDED

#include <vector>

using namespace std;

namespace stream {
    void writeLong(vector<uint8_t>*, uint64_t);
    void writeInt(vector<uint8_t>*, uint32_t);
    void writeShort(vector<uint8_t>*, uint16_t);
    void writeDouble(vector<uint8_t>*, double);

    uint64_t readLong(vector<uint8_t>*, uint64_t*);
    uint32_t readInt(vector<uint8_t>*, uint64_t*);
    uint16_t readShort(vector<uint8_t>*, uint64_t*);
    double readDouble(vector<uint8_t>*, uint64_t*);
}

#endif // STREAM_H_INCLUDED
