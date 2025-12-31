//
//  Author: Tushar Jog
//  Timestamp: <STDF.h Sat 2005/07/23 18:39:16 tjog>
//
//  TODO: Add overloaded assignment operators and copy constructors
//          for all record types.
//


//
//  NOTE: when adding accessor/modifier member functions, try to make
//  the arguments and return values in C++ standard types
//  ie: take string instead of CN
//

#ifndef _STDF_H_
#define _STDF_H_

#include "defines.h"
#include "globals.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

class STDFStream;

class RecordHeader {
    private:
    protected:
        U2 length;
        U1  type;
        U1  subType;
    public:
        RecordHeader(RecordType t);
        RecordHeader(U1 t = 0, U1 st = 0, U2 l = 0);
        void setLength(U2 l);
        void setType(U1 t);
        void setSubType(U1 st);
        void set(U1 t, U1 st, U2 l);

        U2     getLength(void);
        U1     getType(void);
        U1     getSubType(void);
        string getRecordTypeString(void);
        U2     getRecordType(void);

        friend ostream& operator<<(ostream& os, RecordHeader& rh);
        friend STDFStream& operator>>(STDFStream& stdf, RecordHeader& rh);
        friend STDFStream& operator<<(STDFStream& stdf, RecordHeader& rh);
};

class Record {
    public:
        Record(void);
        virtual U2 getLength(void) = 0;

    private:
};

class FARRecord : public Record {
    public:
        // Constructors
        FARRecord(U1 cpu = 2, U1 ver = 4);
        // Accessors
        U1 getCpuType(void);
        U1 getStdfVersion(void);

        // Utility
        RecordHeader getRecordHeader(void);
        U2 getLength(void) { return 2; }

        // Streaming
        friend ostream&      operator<<(ostream&      os, FARRecord& far);
        friend stringstream& operator<<(stringstream& os, FARRecord& far);
        friend STDFStream& operator>>(STDFStream& stdf, FARRecord& far);
        friend STDFStream& operator<<(STDFStream& stdf, FARRecord& far);

    private:
        // Member variables
        U1 cpuType, stdfVersion;
};

class ATRRecord {
    public:
        ATRRecord(U2 len = 0);
        U2 getLength(void);

        friend ostream&    operator<<(ostream&    os,   ATRRecord& atr);
        friend STDFStream& operator>>(STDFStream& stdf, ATRRecord& atr);
        friend STDFStream& operator<<(STDFStream& stdf, ATRRecord& atr);
    private:
        // Member variables
        U4 modTime;
        CN commandLine;
    private:
        U2 recLength;
};

class MIRRecord {
    public:
        MIRRecord(U2 len = 0);
        U2 getLength(void);

        void setLotID( string lot) { lotID = lot; }
        void setPartType( string part) { partType = part; }
        void setNodeName( string node) { nodeName = node; }
        void setTesterType( string tester) { testerType = tester; }
        void setJobName( string job) { jobName = job; }
        void setJobRevision( string revision) { jobRevision = revision; }
        void setSublotID( string sublot) { sublotID = sublot; }
        void setOperatorName( string opName) { operatorName = opName; }
        void setExecutiveType( string exec) { executiveType = exec; }
        friend ostream&    operator<<(ostream&    os,   MIRRecord& mir);
        friend STDFStream& operator>>(STDFStream& stdf, MIRRecord& mir);
        friend STDFStream& operator<<(STDFStream& stdf, MIRRecord& mir);
    private:
        // Member variables
        U4 setupTime, startTime;
        U1 stationNumber;
        C1 modeCode, retestCode, protectionCode;
        U2 burninTime;
        C1 commandCode;
        CN lotID,         partType,         nodeName,      testerType;
        CN jobName,       jobRevision,      sublotID,      operatorName;
        CN executiveType, executiveVersion, testCode,      testTemp;
        CN userText,      auxFile,          packageType,   familyID;
        CN dateCode,      facilityID,       floorID,       processID;
        CN operationFreq, specName,         specVersion,   flowID;
        CN setupID,       designRev,        engineeringID, romCode;
        CN serialNumber,  supervisorName;
    private:
        U2 recLength;

};

class SDRRecord {
    public:
        SDRRecord(U2 len = 0);
        U2 getLength(void);

        friend ostream&    operator<<(ostream&    os,   SDRRecord& sdr);
        friend STDFStream& operator>>(STDFStream& stdf, SDRRecord& sdr);
        friend STDFStream& operator<<(STDFStream& stdf, SDRRecord& sdr);
    private:
        U1 headNumber, siteGroup, siteCount;
        std::vector<U1> siteNumbers;
        CN     handlerType,     handlerID;
        CN        cardType,        cardID;
        CN        loadType,        loadID;
        CN         dibType,         dibID;
        CN       cableType,       cableID;
        CN   contactorType,   contactorID;
        CN       laserType,       laserID;
        CN       extraType,       extraID;
    private:
        U2 recLength;

};

//
//  PMR - Pin Map Record
//
//  channelType is tester platform specific
//  Am defining the following
//      Type    Definition
//      ----    -----------------------------------------
//       0      Not Defined
//       1      Digital Channel
//       2      Digital Channel used to drive Relay
//       4      Analog  Channel
//       8      MS Channel
//      ----    -----------------------------------------

class PMRRecord {
    public:
        PMRRecord(U2 len = 0);
        PMRRecord(U2 ind, string cName, string pName, string lName, U1 hNum=1, U1 sNum=1, U2 cType=0);
        U2 getLength(void);
        RecordHeader getRecordHeader(void);

        enum { NOT_DEFINED=0, DIGITAL=1, RELAY=2, ANALOG=4, MS=8};

        void setIndex        ( U2     ind)  { index        = ind;  }
        void setChannelType  ( U2     type) { channelType  = type; }
        void setChannelName  ( string name) { channelName  = name; }
        void setPhysicalName ( string name) { physicalName = name; }
        void setLogicalName  ( string name) { logicalName  = name; }
        void setHeadNum      ( U1     num)  { headNum      = num;  }
        void setSiteNum      ( U1     num)  { siteNum      = num;  }

        friend ostream&    operator<<(ostream&    os,   PMRRecord& pmr);
        friend STDFStream& operator>>(STDFStream& stdf, PMRRecord& pmr);
        friend STDFStream& operator<<(STDFStream& stdf, PMRRecord& pmr);
    private:
        U2 index;
        U2 channelType;
        CN channelName;
        CN physicalName, logicalName;
        U1 headNum, siteNum;
    private:
        U2 recLength;
};

class PGRRecord {
    public:
        PGRRecord(U2 len = 0);
        U2 getLength(void);
        RecordHeader getRecordHeader(void);

        void setIndex( U2 ind)  { index = ind;  }
        void setGroupName( string name) { groupName = name; }
        void addPMRIndex(U2 ind);

        friend ostream&    operator<<(ostream&    os,   PGRRecord& pgr);
        friend STDFStream& operator>>(STDFStream& stdf, PGRRecord& pgr);
        friend STDFStream& operator<<(STDFStream& stdf, PGRRecord& pgr);
    private:
        U2 index;
        CN groupName;
        U2 indexCount;
        std::vector<U2> pmrIndexes;
    private:
        U2 recLength;
};

class PIRRecord {
    public:
        PIRRecord(U2 len = 0);
        U2 getLength(void);
        
        friend ostream&    operator<<(ostream&    os,   PIRRecord& pir);
        friend STDFStream& operator>>(STDFStream& stdf, PIRRecord& pir);
        friend STDFStream& operator<<(STDFStream& stdf, PIRRecord& pir);
    private:
        U1 headNumber, siteNumber;
    private:
        U2 recLength;
};


class FTRRecord {
    public:
        FTRRecord(U2 len = 0);
        U2 getLength(void);

        friend ostream&    operator<<(ostream&    os,   FTRRecord& ftr);
        friend STDFStream& operator>>(STDFStream& stdf, FTRRecord& ftr);
        friend STDFStream& operator<<(STDFStream& stdf, FTRRecord& ftr);
    private:
        U4 testNumber;
        U1 headNumber,   siteNumber;
        B1 testFlag,     optFlag;
        U4 cycleCount,   relVecAdd,  repeatCount, numFail;
        I4 xFailAdd,     yFailAdd;
        I2 vectorOffset;
        U2 rtnCount,     pgmCount;
        std::vector<U2> rtnIndexes;
        std::vector<N1> rtnStates;
        std::vector<U2> pgmIndexes;
        std::vector<N1> pgmStates;
        DN failPin;
        CN vectorName, timeSet, opCode, testText;
        CN alarmID, progText, resultText;
        //-----------------------------------------
        U1 patGNumber;
        DN spinMap;
    private:
        U2 recLength;
};

class GDRRecord {
    public:
        GDRRecord(U2 len=0);
        U2 getLength(void);

        friend ostream&    operator<<(ostream&    os,   GDRRecord& gdr);
        friend STDFStream& operator>>(STDFStream& stdf, GDRRecord& gdr);
        friend STDFStream& operator<<(STDFStream& stdf, GDRRecord& gdr);
    private:
        U2 fieldCount;
        std::vector<VN> genData;
    private:
        U2 recLength;
};

class PTRRecord {
    public:
        PTRRecord(U2 len = 0);
        U2 getLength(void);

        friend ostream&    operator<<(ostream&    os,   PTRRecord& ptr);
        friend STDFStream& operator>>(STDFStream& stdf, PTRRecord& ptr);
        friend STDFStream& operator<<(STDFStream& stdf, PTRRecord& ptr);
    private:
        U4 testNumber;
        U1 headNumber, siteNumber;
        B1 testFlag, parmFlag;
        R4 result;
        CN testText, alarmID;
        //-----------------------
        B1 optFlag;
        I1 resultScale, llmScale, hlmScale;
        R4 loLimit, hiLimit;
        CN units;
        CN cResFmt, cLLMFmt, cHLMFmt;
        R4 loSpec, hiSpec;
    private:
        U2 recLength;
};

class PRRRecord {
    public:
        PRRRecord(U2 len = 0);
        U2 getLength(void);

        friend ostream&    operator<<(ostream&    os,   PRRRecord& prr);
        friend STDFStream& operator>>(STDFStream& stdf, PRRRecord& prr);
        friend STDFStream& operator<<(STDFStream& stdf, PRRRecord& prr);

    private:
        U1 headNumber, siteNumber;
        B1 partFlag;
        U2 numTest, hardBin, softBin;
        I2 xCoord, yCoord;
        U4 testTime;
        CN partID, partText;
        BN partFix;
    private:
        U2 recLength;
};

class MPRRecord {
    public:
        MPRRecord(U2 len = 0);
        U2 getLength(void);
        friend ostream&    operator<<(ostream&    os,   MPRRecord& mpr);
        friend STDFStream& operator>>(STDFStream& stdf, MPRRecord& mpr);
        friend STDFStream& operator<<(STDFStream& stdf, MPRRecord& mpr);
    private:
        U4 testNumber;
        U1 headNumber, siteNumber;
        B1 testFlag, parmFlag;
        U2 indexCount, resultCount;
        std::vector<N1> rtnStates;
        std::vector<R4> rtnResults;
        CN testText, alarmID;

        //---------------------------------
        B1 optFlag;
        I1 resultScale, llmScale, hlmScale;
        R4 loLimit, hiLimit;
        R4 startIn, incrIn;
        std::vector<U2> pmrIndexes;
        CN units, unitsIn, cResFmt, cLLMFmt, cHLMFmt;
        R4 loSpec, hiSpec;
    private:
        U2 recLength;
};

class TSRRecord {
    public:
        TSRRecord(U2 len = 0);
        U2 getLength(void);

        friend ostream&    operator<<(ostream&    os,   TSRRecord& tsr);
        friend STDFStream& operator>>(STDFStream& stdf, TSRRecord& tsr);
        friend STDFStream& operator<<(STDFStream& stdf, TSRRecord& tsr);
    private:
        U1 headNumber, siteNumber;
        C1 testType;
        U4 testNumber, execCount, failCount, alarmCount;
        CN testName, seqName, testLabel;
        B1 optFlag;
        R4 testTime, testMin, testMax, testSum, testSquare;
    private:
        U2 recLength;
};

class DTRRecord {
    public:
        DTRRecord(U2 len = 0);
        U2 getLength(void);

        friend ostream&    operator<<(ostream&    os,   DTRRecord& dtr);
        friend STDFStream& operator>>(STDFStream& stdf, DTRRecord& dtr);
        friend STDFStream& operator<<(STDFStream& stdf, DTRRecord& dtr);
    private: // Members
        CN text;
    private:
        U2 recLength;
};

class SBRRecord {
    public:
        SBRRecord(U2 len = 0);
        U2 getLength(void);

        friend ostream&    operator<<(ostream&    os,   SBRRecord& sbr);
        friend STDFStream& operator>>(STDFStream& stdf, SBRRecord& sbr);
        friend STDFStream& operator<<(STDFStream& stdf, SBRRecord& sbr);
    private:
        U1 headNumber, siteNumber;
        U2 sbinNumber;
        U4 sbinCount;
        C1 sbinPF;
        CN sbinName;
    private:
        U2 recLength;

};

class HBRRecord {
    public:
        HBRRecord(U2 len = 0);
        U2 getLength(void);

        friend ostream&    operator<<(ostream&    os,   HBRRecord& hbr);
        friend STDFStream& operator>>(STDFStream& stdf, HBRRecord& hbr);
        friend STDFStream& operator<<(STDFStream& stdf, HBRRecord& hbr);
    private:
        U1 headNumber, siteNumber;
        U2 hbinNumber;
        U4 hbinCount;
        C1 hbinPF;
        CN hbinName;
    private:
        U2 recLength;

};

class BPSRecord {
    public:
        BPSRecord(U2 len = 0);
        U2 getLength(void);

        void setSeqName(string name) { seqName = name; }

        friend ostream&    operator<<(ostream&    os,   BPSRecord& bps);
        friend STDFStream& operator>>(STDFStream& stdf, BPSRecord& bps);
        friend STDFStream& operator<<(STDFStream& stdf, BPSRecord& bps);
    private:
        CN seqName;
    private:
        U2 recLength;
};

class EPSRecord {
    public:
        EPSRecord(U2 len = 0);
        U2 getLength(void);

        void setSeqName(string name) { seqName = name; }

        friend ostream&    operator<<(ostream&    os,   EPSRecord& eps);
        friend STDFStream& operator>>(STDFStream& stdf, EPSRecord& eps);
        friend STDFStream& operator<<(STDFStream& stdf, EPSRecord& eps);
    private: // STDF Members -- NONE
    private:
        CN seqName;
        U2 recLength;
};

class MRRRecord {
    public:
        MRRRecord(U2 len = 0);
        U2 getLength(void);

        friend ostream&    operator<<(ostream&    os,   MRRRecord& mrr);
        friend STDFStream& operator>>(STDFStream& stdf, MRRRecord& mrr);
        friend STDFStream& operator<<(STDFStream& stdf, MRRRecord& mrr);
    private: // STDF Members
        U4 finishTime;
        C1 dispCode;
        CN userDesc, execDesc;
    private:
        U2 recLength;
};

class PCRRecord {
    public:
        PCRRecord(U2 len = 0);
        U2 getLength(void);

        friend ostream&    operator<<(ostream&    os,   PCRRecord& pcr);
        friend STDFStream& operator>>(STDFStream& stdf, PCRRecord& pcr);
        friend STDFStream& operator<<(STDFStream& stdf, PCRRecord& pcr);
    private:
        U1 headNumber, siteNumber;
        U4 partCount, retestCount, abortCount, goodCount, funcCount;
    private:
        U2 recLength;
};

class PLRRecord {
    public:
        PLRRecord(U2 len = 0);
        U2 getLength(void);

        friend ostream&    operator<<(ostream&    os,   PLRRecord& plr);
        friend STDFStream& operator>>(STDFStream& stdf, PLRRecord& plr);
        friend STDFStream& operator<<(STDFStream& stdf, PLRRecord& plr);
    private:
        U2 grpCount;
        std::vector<U2> grpIndex, grpMode;
        std::vector<U1> grpRadix;
        std::vector<CN> pgmChar, rtnChar, pgmChal, rtnChal;
};

#endif

