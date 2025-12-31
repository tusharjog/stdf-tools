//
//  Author: Tushar Jog
//  Timestamp: <globals.cpp Tue 2005/07/19 23:17:11 tjog>
//


#include <iostream>
#include "globals.h"

ByteOrder getNativeByteOrder( void ) {
    ByteOrder endianness;
    endianness = NotDefined;
    // Check endian-ness during runtime
    unsigned int endianInt;
    endianInt = 0x04030201;
    char* pUINT = (char *) &endianInt;
    if(pUINT[0] == 0x04) { 
        endianness = BigEndian;
    }
    else if(pUINT[0] == 0x01) { 
        endianness = LittleEndian;
    }
    else if(pUINT[0] == 0x03) { 
        endianness = PDPEndian;
    }

    return endianness;
}

CN::CN(void) {
    count = 0;
    str = "";
}

CN CN::operator= (string s) {
    str = s;
    count = str.length();
    return *this;
}

U2 CN::getLength(void) {
    U2 len = 1 + count;
    return len;
}

/*
U2 CN::operator+ (U2 num) {
    U2 len;
    len = num + count;
    return len;
}
*/

ostream& operator<<(ostream& os, CN& cn) {
#ifdef DEBUG
    os << "[" << (int) cn.count << "] ";
#endif
    os << cn.str;
    return os;
}

DN::DN(void) {
    count = 0;
    data.clear();
}

U2 DN::getLength(void) {
    U2 numBytes = 0;
    numBytes += (count / 8) + 1;
    if((count % 8) ==0) numBytes--;
    numBytes += 2;

    return numBytes;
}

ostream& operator<<(ostream& os, DN& dn) {
#ifdef DEBUG
    os << "[" << (int) dn.count << "] ";
#endif
    
    std::vector<U1>::iterator iter;
    for(iter = dn.data.begin(); iter != dn.data.end(); iter++)
        os << *iter;
    return os;
}

BN::BN(void) {
    count = 0;
    data.clear();
}

U2 BN::getLength(void) {
    U2 len = 0;
    len += 1 + count;

    return len;
}

ostream& operator<<(ostream& os, BN& bn) {
#ifdef DEBUG
    os << "[" << (int) bn.count << "] ";
#endif
    
    std::vector<U1>::iterator iter;
    for(iter = bn.data.begin(); iter != bn.data.end(); iter++)
//        os << *iter;
        os << toBinString( *iter );
    return os;
}

VN::VN(void) {
    code = 255;

    r8 = 0; u4 = 0; // This will assign all bytes of the union to zero
}

U2 VN::getLength(void) {
    U2 len = 0;
    len = 1; // code byte

    switch(code) {
        case 0:  len += 0;              break; // B0
        case 1:  len += 1;              break; // U1
        case 2:  len += 2;              break; // U2
        case 3:  len += 4;              break; // U4
        case 4:  len += 1;              break; // I1
        case 5:  len += 2;              break; // I2
        case 6:  len += 4;              break; // I4
        case 7:  len += 4;              break; // R4
        case 8:  len += 8;              break; // R8
        case 10: len += cn.getLength(); break; // CN
        case 11: len += bn.getLength(); break; // BN
        case 12: len += dn.getLength(); break; // CN
        case 13: len += 1;              break; // N1
        default:
            break;
    };

    return len;
}


ostream& operator<<(ostream& os, VN& gd) {
    switch(gd.code) {
        case 0:  os << "PAD"; break;
        case 1:  os << (int) gd.u1; break;
        case 2:  os << (int) gd.u2; break;
        case 3:  os << (int) gd.u4; break;
        case 4:  os << (int) gd.i1; break;
        case 5:  os << (int) gd.i2; break;
        case 6:  os << (int) gd.i4; break;
        case 7:  os << gd.r4; break;
        case 8:  os << gd.r8; break;
        case 10: os << gd.cn; break;
        case 11: os << gd.bn; break;
        case 12: os << gd.dn; break;
        case 13: os << gd.n1; break;
        default: break;
    };

    return os;
}


//
// Format
//  0       0b00000101
//  1       0b101
//  2       00000101
//  3       101
string toBinString(unsigned char value, int fmt) {
    string result;
    static char	*first_nibble[] = {
        "0",    "1",   "10",   "11",
        "100",  "101",  "110",  "111",
        "1000", "1001", "1010", "1011",
        "1100", "1101", "1110", "1111",
    };
    static char	*next_nibble[] = {
        "0000", "0001", "0010", "0011",
        "0100", "0101", "0110", "0111",
        "1000", "1001", "1010", "1011",
        "1100", "1101", "1110", "1111",
    };
//    char first = value >> 4;
//    char second = value - (first << 4);

//    char first = value & 0xF0;
//    first = first >> 4;
//    char second = value & 0x0F;
    unsigned short first = value & 0xF0;
    first = first >> 4;
    unsigned short second = value & 0x0F;

    if(fmt == 2 || fmt == 4)
        result = "";
    else 
        result = "0b";

    if(fmt == 0 || fmt == 2)
        result += next_nibble[first];
    else if(fmt == 1 || fmt == 3)
        result += first_nibble[first];
    else
        result += next_nibble[first];

    result += next_nibble[second];

    return result;
}

string time2string(int time) {
    time_t temp = time;
    string timeStr = ctime(&temp);
    if(timeStr.size() > 1) {
        timeStr.erase( timeStr.size() - 1, 1);
    }

    return timeStr;
}


