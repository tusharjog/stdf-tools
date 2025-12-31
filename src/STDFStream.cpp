//
//  Author: Tushar Jog
//  Timestamp: <STDFStream.cpp Sun 2005/07/17 16:27:31 tjog>
//

#include <string>
#include <fstream>
#include <assert.h>
#include "STDFStream.h"

//ByteOrder getNativeByteOrder( void );

/*
STDFStream::STDFStream() {
    nativeByteOrder = getNativeByteOrder();
    filename        = "";
    rwMode          = None;
    noswap          = true;
    status          = OK;
    byteCount = 0;
}
*/

STDFStream::STDFStream(string STDFFileName, RWMode mode) {
    filename        = STDFFileName;
    rwMode          = mode;
    nativeByteOrder = getNativeByteOrder();
    byteOrder       = nativeByteOrder;
    noswap          = true;
    status          = OK;
    byteCount       = 0;
    recCount        = 0;
    readCount       = 0;
}

STDFStream::~STDFStream() {
    // Close all io streams here
        if(input.is_open())  input.close();
        if(output.is_open()) output.close();
}

void STDFStream::setByteOrder(ByteOrder order) {
    byteOrder = order;
    if(nativeByteOrder == byteOrder)
        noswap = true;
    else 
        noswap = false;
}
void STDFStream::setMode(RWMode mode) {
    rwMode = mode;
}

void STDFStream::attach(string STDFFileName, RWMode mode) {
    rwMode   = mode; // For some reason this operation is not atomic
                     // hence use mode instead or rwMode for the rest of the member function
    filename = STDFFileName;
//    cerr << "RWMode : " << mode << rwMode << endl;

    if(mode == ReadOnly) {
        // RWMode is ReadOnly, open file for reading and prohibit writing from
        // now on, on this STDFStream
        input.open( STDFFileName.c_str(), ios::in | ios::binary);
        if(! input.is_open()) {
            cerr << "ERROR: Could not open STDF file " << STDFFileName << " for read." << endl;
            cerr << "   In -> " <<  __func__ << endl;
            rwMode = None;
            STDFFileName = "";
        }

    }
    else if(mode == WriteOnly) {
        // RWMode is WriteOnly, open file for writing and prohibit reading from
        // now on, on this STDFStream
        // TODO: For now truncate file to zero if it already exists, later on
        //       Add code to append STDF data to file and update audit trails record.
        output.open( STDFFileName.c_str(), ios::out | ios::trunc | ios::binary);
        if(! output.is_open()) {
            cerr << "ERROR: Could not open STDF file " << STDFFileName << " for write." << endl;
            cerr << "   In -> " <<  __func__ << endl;
            rwMode = None;
            STDFFileName = "";
        }
    }
    else {
        // RWMode is None , wait till first IO operation to set it correctly
    }
}

void STDFStream::setStatus(Status stat) {
    if(status == OK)
        status = stat;

}

void STDFStream::skip(U2 len) {
//    char buf[256*256];
//    if(input.readsome( buf, len) != len) {
//        setStatus(ReadEOF);
//    }

//    assert(getStatus() == OK);

    if((getStatus() == OK) && (len != 0)) {
        input.ignore( len);
        if(input.eof()) 
            setStatus(ReadEOF);
        if(input.fail()) 
            setStatus(ReadFail);
        byteCount += len;
    }

    return;
}
void STDFStream::read(char *buf, U2 len) {
    if((getStatus() == OK) && (len != 0) && (rwMode == ReadOnly)) {
        input.read( buf, len);
//        input.ignore( len);
        if(input.eof()) 
            setStatus(ReadEOF);
        if(input.fail()) 
            setStatus(ReadFail);
        byteCount += len;
    }

    return;
}

STDFStream &STDFStream::operator>>(U1 &i) {
    if(rwMode != ReadOnly) return *this;

    i = 0;
    if((recCount > 0) && (readCount >= recCount)) return *this;

//    assert(getStatus() == OK);

    if(getStatus() == OK) {
        char c;
        c = input.get();
        i = c;
        byteCount++;
        readCount++;
    }

    return *this;
}
STDFStream &STDFStream::operator<<(U1 i) {
    assert(rwMode == WriteOnly);
    if(rwMode != WriteOnly) return *this;

    if(getStatus() == OK) {
        output.put(i);
        if(!input.good()) 
            setStatus(WriteFail);
        byteCount++;
    }

    return *this;
}
STDFStream &STDFStream::operator>>(U2 &i) {
    if(rwMode != ReadOnly) return *this;

    i = 0;
    if((recCount > 0) && (readCount >= recCount)) return *this;

//    assert(getStatus() == OK);
    if(getStatus() == OK) {
        if(noswap) {
            input.read((char *)&i, 2);
            if(!input.good()) {
                i = 0;
                setStatus(ReadEOF);
            }
        }
        else {
            register uchar *p = (uchar *)(&i);
            char buf[2];
            if((input.read(buf, 2)).good()) {
                *p++ = buf[1];
                *p   = buf[0];
            }
            else {
                i = 0;
                setStatus(ReadEOF); 
            }
        }
        byteCount += 2;
        readCount += 2;
    }

    return *this;
}
STDFStream &STDFStream::operator<<(U2 i) {
    assert(rwMode == WriteOnly);
    if(rwMode != WriteOnly) return *this;

    if(getStatus() == OK) {
        output.write( (char *) &i, 2);
        if(!input.good()) 
            setStatus(WriteFail);
        
        byteCount += 2;
    }

    return *this;
}
STDFStream &STDFStream::operator>>(U4 &i) {
    if(rwMode != ReadOnly) return *this;

    i = 0;
    if((recCount > 0) && (readCount >= recCount)) return *this;

//    assert(getStatus() == OK);
    if(getStatus() == OK) {

        if(noswap) {
            input.read((char *)&i, 4);
            if(!input.good()) {
                i = 0;
                setStatus(ReadEOF);
            }
        }
        else {
            register uchar *p = (uchar *)(&i);
            char buf[4];
            if((input.read(buf, 4)).good()) {
                *p++ = buf[3];
                *p++ = buf[2];
                *p++ = buf[1];
                *p   = buf[0];
            }
            else  setStatus(ReadEOF); 
        }
        byteCount += 4;
        readCount += 4;
    }

    return *this;
}

STDFStream &STDFStream::operator<<(U4 i) {
    assert(rwMode == WriteOnly);
    if(rwMode != WriteOnly) return *this;

    if(getStatus() == OK) {
        output.write( (char *) &i, 4);
        if(!input.good()) 
            setStatus(WriteFail);
        
        byteCount += 4;
    }

    return *this;
}
STDFStream &STDFStream::operator>>(I2 &i) {
    if(rwMode != ReadOnly) return *this;

    i = 0;
    if((recCount > 0) && (readCount >= recCount)) return *this;

    if(getStatus() == OK) {
        if(noswap) {
            input.read((char *)&i, 2);
            if(!input.good()) {
                i = 0;
                setStatus(ReadEOF);
            }
        }
        else {
            register char *p = (char *)(&i);
            char buf[2];
            if((input.read(buf, 2)).good()) {
                *p++ = buf[1];
                *p   = buf[0];
            }
            else {
                i = 0;
                setStatus(ReadEOF); 
            }
        }
        byteCount += 2;
        readCount += 2;
    }
    return *this;
}
STDFStream &STDFStream::operator<<(I2 i) {
    assert(rwMode == WriteOnly);
    if(rwMode != WriteOnly) return *this;

    if(getStatus() == OK) {
        output.write( (char *) &i, 2);
        if(!input.good()) 
            setStatus(WriteFail);
        
        byteCount += 2;
    }

    return *this;
}
STDFStream &STDFStream::operator>>(I4 &i) {
    if(rwMode != ReadOnly) return *this;

    i = 0;
    if((recCount > 0) && (readCount >= recCount)) return *this;

    if(getStatus() == OK) {

        if(noswap) {
            input.read((char *)&i, 4);
            if(!input.good()) {
                i = 0;
                setStatus(ReadEOF);
            }
        }
        else {
            register char *p = (char *)(&i);
            char buf[4];
            if((input.read(buf, 4)).good()) {
                *p++ = buf[3];
                *p++ = buf[2];
                *p++ = buf[1];
                *p   = buf[0];
            }
            else  setStatus(ReadEOF); 
        }
        byteCount += 4;
        readCount += 4;
    }
    return *this;
}
STDFStream &STDFStream::operator<<(I4 i) {
    assert(rwMode == WriteOnly);
    if(rwMode != WriteOnly) return *this;

    if(getStatus() == OK) {
        output.write( (char *) &i, 4);
        if(!input.good()) 
            setStatus(WriteFail);

        byteCount += 4;
    }
        
    return *this;
}

STDFStream &STDFStream::operator>>(R4 &i) {
    if(rwMode != ReadOnly) return *this;

    i = 0.0;
    if((recCount > 0) && (readCount >= recCount)) return *this;

    //    assert(getStatus() == OK);

    if(getStatus() == OK) {
        if(noswap) {
            input.read((char *)&i, 4);
            if(!input.good()) {
                i = 0.0;
                setStatus(ReadEOF);
            }
        }
        else {
            register uchar *p = (uchar *)(&i);
            char buf[4];
            if((input.read(buf, 4)).good()) {
                *p++ = buf[3];
                *p++ = buf[2];
                *p++ = buf[1];
                *p   = buf[0];
            }
            else  setStatus(ReadEOF); 
        }
        byteCount += 4;
        readCount += 4;
    }
    return *this;
}
STDFStream &STDFStream::operator<<(R4 i) {
    assert(rwMode == WriteOnly);
    if(rwMode != WriteOnly) return *this;

    if(getStatus() == OK) {
        output.write( (char *) &i, 4);
        if(!input.good()) 
            setStatus(WriteFail);
        
        byteCount += 4;
    }

    return *this;
}

STDFStream &STDFStream::operator>>(R8 &i) {
    if(rwMode != ReadOnly) return *this;

    i = 0.0;
    if((recCount > 0) && (readCount >= recCount)) return *this;

//    assert(getStatus() == OK);
    if(getStatus() == OK) {

        if(noswap) {
            input.read((char *)&i, 8);
            if(!input.good()) {
                i = 0.0;
                setStatus(ReadEOF);
            }
        }
        else {
            register uchar *p = (uchar *)(&i);
            char buf[8];
            if((input.read(buf, 8)).good()) {
                *p++ = buf[7];
                *p++ = buf[6];
                *p++ = buf[5];
                *p++ = buf[4];
                *p++ = buf[3];
                *p++ = buf[2];
                *p++ = buf[1];
                *p   = buf[0];
            }
            else  setStatus(ReadEOF); 
        }
        byteCount += 8;
        readCount += 8;
    }
    return *this;
}
STDFStream &STDFStream::operator<<(R8 i) {
    assert(rwMode == WriteOnly);
    if(rwMode != WriteOnly) return *this;

    if(getStatus() == OK) {
        output.write( (char *) &i, 8);
        if(!input.good()) 
            setStatus(WriteFail);
        
        byteCount += 8;
    }

    return *this;
}

STDFStream &STDFStream::operator>>(C1 &i) {
    if(rwMode != ReadOnly) return *this;

    i = 0;
    if((recCount > 0) && (readCount >= recCount)) return *this;

    if(getStatus() == OK) {
        i = input.get();
        byteCount++;
        readCount++;
    }

    return *this;
}

STDFStream &STDFStream::operator>>(CN &i) {
    if(rwMode != ReadOnly) return *this;

    int len = 0;
    i.count = 0;
    i.str = "";
    if((recCount > 0) && (readCount >= recCount)) return *this;

    if(getStatus() == OK) {
        len = input.get();
        if(len > 0) {
            char buf[len + 1];
            input.read( buf, len);
            if(!input.good()) {
                setStatus(ReadEOF);
            }
            else {
                i.count = len;
                for(int j=0; j<len; j++)
                    i.str += buf[j];
            }
        }
        byteCount += i.count + 1;
        readCount += i.count + 1;
    }

    return *this;
}

STDFStream &STDFStream::operator>>(DN &i) {
    if(rwMode != ReadOnly) return *this;

    i.count = 0;
    i.data.clear();
    if((recCount > 0) && (readCount >= recCount)) return *this;

    if(getStatus() == OK) {
        U2 len; // NOTE: length in bits not bytes
        *this >> len;
        i.count = len;

        U2 numBytes = (len / 8) + 1;
        if((len % 8) == 0) numBytes--;

        if(len > 0) {


            char buf[numBytes];
            if((input.read(buf, numBytes)).good()) {
                for(unsigned int j = 0; j < numBytes; j++)
                    i.data.push_back(buf[j]);
            }
            else  setStatus(ReadEOF); 
        }
        byteCount += numBytes;
        readCount += numBytes;
    }
    return *this;
}

STDFStream &STDFStream::operator>>(BN &i) {
    if(rwMode != ReadOnly) return *this;

    i.count = 0;
    i.data.clear();
    if((recCount > 0) && (readCount >= recCount)) return *this;

    if(getStatus() == OK) {
        U1 len; // length in bytes
        *this >> len;
        i.count = len;
        if(len > 0) {

            char buf[len];
            if((input.read(buf, len)).good()) {
                for(unsigned int j = 0; j < len; j++)
                    i.data.push_back(buf[j]);
            }
            else  setStatus(ReadEOF); 
        }
        byteCount += i.count;
        readCount += i.count;
    }
    return *this;
}

STDFStream &STDFStream::operator>>(VN &i) {
    if(rwMode != ReadOnly) return *this;

    i.code = 0;
    if((recCount > 0) && (readCount >= recCount)) return *this;

    if(getStatus() == OK) {
        *this >> i.code;
        switch(i.code) {
            case 0: break;
            case 1:  *this >> i.u1; break;
            case 2:  *this >> i.u2; break;
            case 3:  *this >> i.u4; break;
            case 4:  *this >> i.i1; break;
            case 5:  *this >> i.i2; break;
            case 6:  *this >> i.i4; break;
            case 7:  *this >> i.r4; break;
            case 8:  *this >> i.r8; break;
            case 10: *this >> i.cn; break;
            case 11: *this >> i.bn; break;
            case 12: *this >> i.dn; break;
            case 13: *this >> i.n1; break;
            default: break;
        };
        if(!input.good()) 
            setStatus(ReadFail);
    }
    return *this;
}

STDFStream &STDFStream::operator<<(VN i) {
    assert(rwMode == WriteOnly);
    if(rwMode != WriteOnly) return *this;

    if(getStatus() == OK) {
        *this << i.code;
        switch(i.code) {
            case 0: break;
            case 1:  *this << i.u1; break;
            case 2:  *this << i.u2; break;
            case 3:  *this << i.u4; break;
            case 4:  *this << i.i1; break;
            case 5:  *this << i.i2; break;
            case 6:  *this << i.i4; break;
            case 7:  *this << i.r4; break;
            case 8:  *this << i.r8; break;
            case 10: *this << i.cn; break;
            case 11: *this << i.bn; break;
            case 12: *this << i.dn; break;
            case 13: *this << i.n1; break;
            default: break;
        };
        if(!output.good()) 
            setStatus(WriteFail);
    }
    return *this;
}

STDFStream &STDFStream::operator<<(C1 i) {
    assert(rwMode == WriteOnly);
    if(rwMode != WriteOnly) return *this;

    if(getStatus() == OK) {
        output.put(i);
        if(!output.good()) 
            setStatus(WriteFail);
    }

    return *this;
}
STDFStream &STDFStream::operator<<(CN i) {
    assert(rwMode == WriteOnly);
    if(rwMode != WriteOnly) return *this;

    if(getStatus() == OK) {
        output.put(i.count);
        for(unsigned char j = 0; j<i.count; j++)
            output.put(i.str[j]);
        if(!output.good()) 
            setStatus(WriteFail);
    }

    return *this;
}

STDFStream &STDFStream::operator<<(DN i) {
    assert(rwMode == WriteOnly);
    if(rwMode != WriteOnly) return *this;

    if(getStatus() == OK) {
        output.write( (char *) &(i.count), 2);

//        U2 numBytes = (i.count / 8) + 1;
//        if((i.count % 8) == 0) numBytes--;
        U2 numBytes = i.getLength() - 2;

        for(unsigned char j = 0; j<numBytes; j++)
            output.put(i.data[j]);
        if(!output.good()) 
            setStatus(WriteFail);
    }

    return *this;
}

STDFStream &STDFStream::operator<<(BN i) {
    assert(rwMode == WriteOnly);
    if(rwMode != WriteOnly) return *this;

    if(getStatus() == OK) {
        output.put(i.count);

        U2 numBytes = i.getLength() - 1;

        for(unsigned char j = 0; j<numBytes; j++)
            output.put(i.data[j]);
        if(!output.good()) 
            setStatus(WriteFail);
    }

    return *this;
}

/*
STDFStream &STDFStream::operator>>(N1 &i) {
    if(rwMode != ReadOnly) return *this;

    i = 0;

    if(getStatus() == OK) {
        char c;
        c = input.get();
        i = c;
        i = i & 0xF0;
    }

    return *this;
}
STDFStream &STDFStream::operator<<(N1 i) {
    assert(rwMode == WriteOnly);
    if(rwMode != WriteOnly) return *this;

    if(getStatus() == OK) {
        output.put(i);
        if(!input.good()) 
            setStatus(WriteFail);
    }

    return *this;
}
*/

/*
STDFStream &STDFStream::operator>>(RecordHeader &i) {
    if(rwMode != ReadOnly) return *this;

    if(getStatus() == OK) {
        U2 len = 0; U1 t = 0; U1 st = 0;
        *this >> len >> t >> st;
        i.set(t, st, len);
    }

    return *this;
}
*/


string getRecordTypeString(U1 type, U1 subtype) {
    string s;

    if(type == 0) {
        switch(subtype) {
            case 10 : s = "FAR"; break;
            case 20 : s = "ATR"; break;
            default: s = "<>";
        }
    }
    else if(type == 1) {
        switch(subtype) {
            case 10 : s = "MIR"; break;
            case 20 : s = "MRR"; break;
            case 30 : s = "PCR"; break;
            case 40 : s = "HBR"; break;
            case 50 : s = "SBR"; break;
            case 60 : s = "PMR"; break;
            case 62 : s = "PGR"; break;
            case 63 : s = "PLR"; break;
            case 70 : s = "RDR"; break;
            case 80 : s = "SDR"; break;
            default: s = "<>";
        }
    }
    else if(type == 2) {
        switch(subtype) {
            case 10 : s = "WIR"; break;
            case 20 : s = "WRR"; break;
            case 30 : s = "WCR"; break;
            default: s = "<>";
        }
    }
    else if(type == 5) {
        switch(subtype) {
            case 10 : s = "PIR"; break;
            case 20 : s = "PRR"; break;
            default: s = "<>";
        }
    }
    else if(type == 10) {
        switch(subtype) {
            case 30 : s = "TSR"; break;
            default: s = "<>";
        }
    }
    else if(type == 15) {
        switch(subtype) {
            case 10 : s = "PTR"; break;
            case 15 : s = "MPR"; break;
            case 20 : s = "FTR"; break;
            default: s = "<>";
        }
    }
    else if(type == 20) {
        switch(subtype) {
            case 10 : s = "BPS"; break;
            case 20 : s = "EPS"; break;
            default: s = "<>";
        }
    }
    else if(type == 50) {
        switch(subtype) {
            case 10 : s = "GDR"; break;
            case 30 : s = "DTR"; break;
            default: s = "<>";
        }
    }
    else if(type == 180) {
        s = "Reserved for use by Image software";
    }
    else if(type == 181) {
        s = "Reserved for use by IG900 software";
    }
    else {
        s = "<>";
    }
    return s;

}

