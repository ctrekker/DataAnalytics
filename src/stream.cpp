#include <iostream>
#include <vector>

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
        union {
            double dValue;
            uint64_t iValue;
        } dToL;
        dToL.dValue=d;
        writeLong(out, dToL.iValue);
    }
}
