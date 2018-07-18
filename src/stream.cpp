#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

namespace stream {
    void writeLong(vector<uint8_t>* out, uint64_t l) {
        uint8_t currentByte = 0;
        for(int8_t bit=63; bit>=0; bit--) {
            uint8_t currentBit = (l >> bit) & 1;
            uint8_t pos = (bit)%8;
            currentByte |= currentBit << pos;
            if(pos==0) {
                out->push_back(currentByte);
                currentByte = 0;
            }
        }
    }
    void writeInt(vector<uint8_t>* out, uint32_t i) {
        uint8_t currentByte = 0;
        for(int8_t bit=31; bit>=0; bit--) {
            uint8_t currentBit = (i >> bit) & 1;
            uint8_t pos = (bit)%8;
            currentByte |= currentBit << pos;
            if(pos==0) {
                out->push_back(currentByte);
                currentByte = 0;
            }
        }
    }
    void writeShort(vector<uint8_t>* out, uint16_t s) {
        uint8_t currentByte = 0;
        for(int8_t bit=15; bit>=0; bit--) {
            uint8_t currentBit = (s >> bit) & 1;
            uint8_t pos = (bit)%8;
            currentByte |= currentBit << pos;
            if(pos==0) {
                out->push_back(currentByte);
                currentByte = 0;
            }
        }
    }
    void writeDouble(vector<uint8_t>* out, double d) {
        uint64_t u;
        memcpy(&u, &d, sizeof(d));
        writeLong(out, u);
    }

    /*
    void writeLong(vector<uint8_t>* out, uint64_t l) {
        uint8_t currentByte = 0;
        for(int8_t bit=63; bit>=0; bit--) {
            uint8_t currentBit = (l >> bit) & 1;
            uint8_t pos = (bit)%8;
            currentByte |= currentBit << pos;
            if(pos==0) {
                out->push_back(currentByte);
                currentByte = 0;
            }
        }
    }
    */
    uint64_t readLong(vector<uint8_t>* in, uint64_t* cp) {
        uint64_t out = 0;
        for(int8_t byte=7; byte>=0; byte--) {
            uint8_t currentByte = (*in)[(*cp)+byte];
            out |= (uint64_t(currentByte) << ((7-byte)*8));
        }
        (*cp)+=8;
        return out;
    }
    uint32_t readInt(vector<uint8_t>* in, uint64_t* cp) {
        uint32_t out = 0;
        for(int8_t byte=3; byte>=0; byte--) {
            uint8_t currentByte = (*in)[(*cp)+byte];
            out |= currentByte << ((3-byte)*8);
        }
        (*cp)+=4;
        return out;
    }
    uint16_t readShort(vector<uint8_t>* in, uint64_t* cp) {
        uint16_t out = 0;
        for(int8_t byte=1; byte>=0; byte--) {
            uint8_t currentByte = (*in)[(*cp)+byte];
            out |= currentByte << ((1-byte)*8);
        }
        (*cp)+=2;
        return out;
    }
    double readDouble(vector<uint8_t>* in, uint64_t* cp) {
        double d;
        uint64_t u = readLong(in, cp);
        memcpy(&d, &u, sizeof(d));
        return d;
    }
}
