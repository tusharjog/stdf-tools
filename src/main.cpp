//
//  Author: Tushar Jog
//  Timestamp: <main.cpp Sun 2005/07/17 16:46:12 tjog>
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "STDF.h"
#include "STDFStream.h"

using namespace std;

int main(int argc, char** argv) {

    STDFStream istdf, ostdf;
    RecordHeader rec;

    string filename;
    if(argc > 1)
        filename = argv[1];

    istdf.attach( filename,    STDFStream::ReadOnly);
    ostdf.attach( "temp.stdf", STDFStream::WriteOnly);

    istdf >> rec;
    rec.setLength(2); // For FAR length is always 2

    FARRecord far;
    istdf >> far;
    
    if(far.getCpuType() == 0) {
        cerr << "ERROR: Sorry DEC PDP-11 and VAX processors not supported." << endl;
        exit(1);
    }
    else if(far.getCpuType() == 1) {
        istdf.setByteOrder(BigEndian);
    }
    else if(far.getCpuType() == 2) {
        istdf.setByteOrder(LittleEndian);
    }
    ostdf << far;

    cout << rec << endl;
    cout << far << endl;

    unsigned long readCount = 0, writeCount = 0;
    unsigned long startCount = 0, stopCount = 0;
    while(istdf.getStatus() == STDFStream::OK) {
        unsigned int bytesRead = 0;

        istdf >> rec;
        startCount = istdf.getByteCount();
        cout << rec << endl;
        string type = rec.getRecordTypeString();
        U2 fullType = rec.getRecordType();

        switch(fullType) {
            case 10: cout << "FAR" << endl; break;
            case 266: // MIR
                      break;
            default: break;
        };

        if(type == "MIR") {
            MIRRecord mir(rec.getLength());
            istdf >> mir;
            cout  << mir;
            ostdf << mir;
        }
        else if(type == "ATR") {
            ATRRecord atr(rec.getLength());
            istdf >> atr;
            cout  << atr;
            ostdf << atr;
        }
        else if(type == "SDR") {
            SDRRecord sdr(rec.getLength());
            istdf >> sdr;
            cout  << sdr;
            ostdf << sdr;
        }
        else if(type == "PMR") {
            PMRRecord pmr(rec.getLength());
            istdf >> pmr;
            cout  << pmr;
            ostdf << pmr;
        }
        else if(type == "PGR") {
            PGRRecord pgr(rec.getLength());
            istdf >> pgr;
            cout  << pgr;
            ostdf << pgr;
        }
        else if(type == "PIR") {
            PIRRecord pir(rec.getLength());
            istdf >> pir;
            cout  << pir;
            ostdf << pir;
        }
        else if(type == "FTR") {
            FTRRecord ftr(rec.getLength());
            istdf >> ftr;
            cout  << ftr;
            ostdf << ftr;
        }
        else if(type == "GDR") {
            GDRRecord gdr(rec.getLength());
            istdf >> gdr;
            cout  << gdr;
            ostdf << gdr;
        }
        else if(type == "PTR") {
            PTRRecord ptr(rec.getLength());
            istdf >> ptr;
            cout  << ptr;
            ostdf << ptr;
        }
        else if(type == "PRR") {
            PRRRecord prr(rec.getLength());
            istdf >> prr;
            cout  << prr;
            ostdf << prr;
        }
        else if(type == "MPR") {
            MPRRecord mpr(rec.getLength());
            istdf >> mpr;
            cout  << mpr;
            ostdf << mpr;
        }
        else if(type == "TSR") {
            TSRRecord tsr(rec.getLength());
            istdf >> tsr;
            cout  << tsr;
            ostdf << tsr;
        }
        else if(type == "DTR") {
            DTRRecord dtr(rec.getLength());
            istdf >> dtr;
            cout  << dtr;
            ostdf << dtr;
        }
        else if(type == "SBR") {
            SBRRecord sbr(rec.getLength());
            istdf >> sbr;
            cout  << sbr;
            ostdf << sbr;
        }
        else if(type == "HBR") {
            HBRRecord hbr(rec.getLength());
            istdf >> hbr;
            cout  << hbr;
            ostdf << hbr;
        }
        else if(type == "BPS") {
            BPSRecord bps(rec.getLength());
            istdf >> bps;
            cout  << bps;
            ostdf << bps;
        }
        else if(type == "EPS") {
            EPSRecord eps(rec.getLength());
            istdf >> eps;
            cout  << eps;
            ostdf << eps;
        }
        else if(type == "MRR") {
            MRRRecord mrr(rec.getLength());
            istdf >> mrr;
            cout  << mrr;
            ostdf << mrr;
        }
        else if(type == "PCR") {
            PCRRecord pcr(rec.getLength());
            istdf >> pcr;
            cout  << pcr;
            ostdf << pcr;
        }
        else {
            cout << "\tTODO" << endl;
            istdf.skip(rec.getLength());
        }
        bytesRead = istdf.getByteCount() - startCount;

        if(rec.getLength() > bytesRead) {
            cerr << "ERROR: Reading " << type << ", length mismatch. " 
                << bytesRead << " vs " << rec.getLength() << endl;
            cerr << "WARNING: Recovering from previous error." << endl;
            istdf.skip(rec.getLength() - bytesRead);
        }
        else if(rec.getLength() < bytesRead) {
            cerr << "ERROR: Reading " << type << ", length mismatch. " 
                << bytesRead << " vs " << rec.getLength() << endl;
            istdf.close();
            ostdf.close();
            exit(1);
        }

        writeCount = ostdf.getByteCount();
        stopCount = istdf.getByteCount();
        readCount = stopCount - startCount;
    }

    if(istdf.getStatus() == STDFStream::ReadFail) {
        cerr << "ERROR: Read fail for file." << endl;
    }

    return 1;
}


