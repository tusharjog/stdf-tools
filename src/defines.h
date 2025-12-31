//
//  Author: Tushar Jog
//  Timestamp: <defines.h Fri 2005/07/08 17:00:03 tjog>
//

#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <string>

using namespace std;

enum ByteOrder{ 
    NotDefined,
    BigEndian    = 4321,
    LittleEndian = 1234,
    PDPEndian    = 3412
};

enum RecordType {
    Header,
    FAR, ATR, MIR, MRR, PCR, HBR, SBR, PMR,
    PGR, PLR, RDR, SDR, WIR, WRR, WCR, PIR,
    PRR, TSR, PTR, MPR, FTR, BPS, EPS, GDR,
    DTR
};

typedef unsigned char  UCHAR;
typedef unsigned short USHORT;
typedef unsigned long  ULONG;

typedef unsigned char  U1;
typedef unsigned short U2;
typedef unsigned long  U4;
typedef char           I1;
typedef short          I2;
typedef long           I4;
typedef float          R4;
typedef double         R8;

typedef char C1;
typedef unsigned char B1;
typedef unsigned char N1;


typedef unsigned char  uchar;

#endif

/*
   STDF Data Types
-----------|----------------------------------------|-----------------------
   Code    | Description                            | C Type Specifier 
-----------|----------------------------------------|-----------------------
   C*12    | Fixed length character string          | char[12]
           | If a fixed length char string does not |
           | fill the entire field, it must be left |
           | justified and padded with spaces.      |
   C*n     | Variable length character string       |
   C*f     | Variable length character string       | char[]
   U*1     | One byte unsigned integer              | unsigned char
   U*2     | Two byte unsigned integer              | unsigned short
   U*4     | Four byte unsigned integer             | unsigned long
   I*1     | One byte signed integer                | char
   I*2     | Two byte signed integer                | short
   I*4     | Four byte signed integer               | long
   R*4     | Four byte floating point number        | float
   R*8     | Eight byte floating point number       | long float (double)
   B*6     | Fixed length bit-encoded data          | char[6]
   V*n     | Variable data type field               |
           | The data type is specified by a code   |
           | in the first byte, and the data        |
           | follows (max. of 255 bytes)            |
   B*n     | Variable length bit-encoded field      |
           | First byte = unsigned count of bytes   |
           | to follow (max of 255 bytes)           |
   D*n     | Variable length bit-encoded field      | char[]
           | First two bytes = unsigned count       |
           | of bits to follow (max. of 65535 bits) |
           | First data item in least significant   |
           | bit of the third byte of the array     |
           | (first 2 bytes are count)              |
   N*1     | Unsigned integer data stored in a      | char
           | nibble. (Nibble = 4 bits of a byte)    |
           | First item in low 4 bits, second item  |
           | in high 4 bits.                        |
   kxTYPE  | Arry of data of the type specified     | TYPE[]
           | The value of 'k' (the number of        |
           | elements in the array) is defined in   |
           | an earlier field in the record.        |
-----------|----------------------------------------|-----------------------
*/



