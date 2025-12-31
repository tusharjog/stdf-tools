//
//  Author: Tushar Jog
//  Timestamp: <globals.h Fri 2005/07/22 16:58:37 tjog>
//

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "defines.h"
#include <vector>
#include <ctime>

#define DEBUG 1

ByteOrder getNativeByteOrder( void );
string toBinString(unsigned char value, int fmt = 0);

string time2string(int time);

// Maybe put this in STDF.h STDF.cpp
class CN {
    public:
        CN(void);
        U2 getLength(void);
        CN operator= (string s);
//        U2 operator+(U2 num);
        friend ostream& operator<<(ostream& os, CN& cn);

    public:
        char count;
        string str;
};

///////////////////////////////////////////////////////////////////
//
//  D*n     Variable length bit-encoded field           char[]
//          First two bytes = unsigned count of bits to follow
//          (maximum of 65,535 bits)
//          First data item in least significant bit of the
//          third byte of the array (first two bytes are count)
//          Unused bits at the high order end of the last byte
//          must be zero.
//
class DN {
    public:
        DN(void);
        U2 getLength(void); // Returns the number of bytes

        friend ostream& operator<<(ostream& os, DN& cn);
    public:
        U2 count; // count for the number of bits
        std::vector<U1> data;
};

/////////////////////////////////////////////////////////////////////////
//
//  B*n     Variable length bit-encoded field
//          First byte = unsigned count of bytes to follow (max. 255)
//          First data item in least significant bit of the second
//          byte of the array (first byte is count)
//
class BN {
    public:
        BN(void);
        U2 getLength(void);

        friend ostream& operator<<(ostream& os, BN& bn);
    public:
        U1 count;
        std::vector<U1> data;
};

class VN {
    public:
        VN(void);
        U2 getLength(void); // Returns the number of bytes

        friend ostream& operator<<(ostream& os, VN& gd);
    public:
        U1 code;
        union {
            U1 u1;
            U2 u2;
            U4 u4;
            I1 i1;
            I2 i2;
            I4 i4;
            R4 r4;
            R8 r8;
            N1 n1;
        };
        CN cn;
        BN bn;
        DN dn;
};

#endif


