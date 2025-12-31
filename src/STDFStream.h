//
//  Author: Tushar Jog
//  Timestamp: <STDFStream.h Sun 2005/07/17 16:44:29 tjog>
//


#ifndef _STDFSTREAM_H_
#define _STDFSTREAM_H_

#include <iostream>
#include <fstream>
#include <string>
#include "defines.h"
#include "globals.h"
#include "STDF.h"

using namespace std;

string getRecordTypeString(U1 type, U1 subtype);

class STDFStream {
    public:
        enum RWMode { None, ReadOnly, WriteOnly = 2 };
        enum Status { OK, ReadEOF, ReadFail, WriteFail };
//        STDFStream();
        STDFStream(string STDFFileName = "", RWMode mode = None);
        ~STDFStream(); // Destructor

        ByteOrder getByteOrder(void) { return byteOrder; }
        void setByteOrder(ByteOrder order);
        void attach(string STDFFileName, RWMode );
        void setStatus(Status stat);
        void skip(U2 len);
        void read(char *buf, U2 len = 0);
        unsigned long getByteCount(void) { return byteCount; }
        void close(void) { input.close(); output.close(); }
        void setRecCount(U2 len) { if(len > 0) recCount = len; else recCount = 0;
            readCount = 0;
        }

        STDFStream &operator>>(U1 &i);
        STDFStream &operator>>(U2 &i);
        STDFStream &operator>>(U4 &i);
        STDFStream &operator>>(R4 &i);
        STDFStream &operator>>(R8 &i);
        STDFStream &operator>>(C1 &i);
        STDFStream &operator>>(CN &i);
        STDFStream &operator>>(DN &i);
        STDFStream &operator>>(BN &i);
//        STDFStream &operator>>(N1 &i);
        STDFStream &operator>>(I2 &i);
        STDFStream &operator>>(I4 &i);
        STDFStream &operator>>(VN &i);

        STDFStream &operator<<(U1 i);
        STDFStream &operator<<(U2 i);
        STDFStream &operator<<(U4 i);
        STDFStream &operator<<(R4 i);
        STDFStream &operator<<(R8 i);
        STDFStream &operator<<(C1 i);
        STDFStream &operator<<(CN i);
        STDFStream &operator<<(DN i);
        STDFStream &operator<<(BN i);
//        STDFStream &operator<<(N1 i);
        STDFStream &operator<<(I2 i);
        STDFStream &operator<<(I4 i);
        STDFStream &operator<<(VN i);

//        STDFStream &operator>>(RecordHeader     &i);
       
        Status getStatus(void) { return status; }
    protected:
        void setMode(RWMode mode);

    private:
        ByteOrder byteOrder;
        ByteOrder nativeByteOrder;
        bool      noswap;
        RWMode    rwMode;
        ifstream  input;
        ofstream  output;
        std::string    filename;
        Status status;
        unsigned long byteCount;
        U2 recCount, readCount;

};


#endif


