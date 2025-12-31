//
//  Author: Tushar Jog
//  Timestamp: <STDF.cpp Sat 2005/07/23 18:19:03 tjog>
//

#include <ctime>
#include "STDF.h"
#include "STDFStream.h"

using namespace std;

RecordHeader::RecordHeader(RecordType t) {
    length = 0;
    switch(t) {
        case FAR: type = 0;  subType = 10; break;
        case ATR: type = 0;  subType = 20; break;

        case MIR: type = 1;  subType = 10; break;
        case MRR: type = 1;  subType = 20; break;
        case PCR: type = 1;  subType = 30; break;
        case HBR: type = 1;  subType = 40; break;
        case SBR: type = 1;  subType = 50; break;
        case PMR: type = 1;  subType = 60; break;
        case PGR: type = 1;  subType = 62; break;
        case PLR: type = 1;  subType = 63; break;
        case RDR: type = 1;  subType = 70; break;
        case SDR: type = 1;  subType = 80; break;

        case WIR: type = 2;  subType = 10; break;
        case WRR: type = 2;  subType = 20; break;
        case WCR: type = 2;  subType = 30; break;

        case PIR: type = 5;  subType = 10; break;
        case PRR: type = 5;  subType = 20; break;

        case TSR: type = 10;  subType = 30; break;

        case PTR: type = 15; subType = 10; break;
        case MPR: type = 15; subType = 15; break;
        case FTR: type = 15; subType = 20; break;

        case BPS: type = 20; subType = 10; break;
        case EPS: type = 20; subType = 20; break;

        case GDR: type = 50; subType = 10; break;
        case DTR: type = 50; subType = 30; break;

        default:  type = 0;  subType = 0;  break;
    };
}

RecordHeader::RecordHeader(U1 t, U1 st, U2 l) {
    length  = l;
    type    = t;
    subType = st;
}

void RecordHeader::set(U1 t, U1 st, U2 l) {
    type = t;
    subType = st;
    length = l;
}

void RecordHeader::setLength(U2 l) {
    length = l;
}

void RecordHeader::setType(U1 t) {
    type = t;
}

void RecordHeader::setSubType(U1 st) {
    subType = st;
}

U2 RecordHeader::getLength(void) {
    return length;
}

U1 RecordHeader::getType(void) {
    return type;
}

U1 RecordHeader::getSubType(void) {
    return subType;
}

U2 RecordHeader::getRecordType(void) {
    U2 fullType = 0;

    fullType = type << 8;
    fullType += subType;
    return fullType;
}

string RecordHeader::getRecordTypeString(void) {
    string s;

    if(type == 0) {
        switch(subType) {
            case 10 : s = "FAR"; break;
            case 20 : s = "ATR"; break;
            default: s = "<>";
        }
    }
    else if(type == 1) {
        switch(subType) {
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
        switch(subType) {
            case 10 : s = "WIR"; break;
            case 20 : s = "WRR"; break;
            case 30 : s = "WCR"; break;
            default: s = "<>";
        }
    }
    else if(type == 5) {
        switch(subType) {
            case 10 : s = "PIR"; break;
            case 20 : s = "PRR"; break;
            default: s = "<>";
        }
    }
    else if(type == 10) {
        switch(subType) {
            case 30 : s = "TSR"; break;
            default: s = "<>";
        }
    }
    else if(type == 15) {
        switch(subType) {
            case 10 : s = "PTR"; break;
            case 15 : s = "MPR"; break;
            case 20 : s = "FTR"; break;
            default: s = "<>";
        }
    }
    else if(type == 20) {
        switch(subType) {
            case 10 : s = "BPS"; break;
            case 20 : s = "EPS"; break;
            default: s = "<>";
        }
    }
    else if(type == 50) {
        switch(subType) {
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


ostream& operator<<(ostream& os, RecordHeader& rh) {
    return os << (int) rh.length << "\t" 
        << (int) rh.type << "\t" 
        << (int) rh.subType << "\t" 
        << rh.getRecordTypeString();
}

STDFStream& operator>>(STDFStream& stdf, RecordHeader& rh) {
    stdf >> rh.length >> rh.type >> rh.subType;
    return stdf;
}
STDFStream& operator<<(STDFStream& stdf, RecordHeader& rh) {
    stdf << rh.length << rh.type << rh.subType;
    return stdf;
}

Record::Record(void) {
}

//U2 Record::getLength(void) { U2 len = 0; return len; }

FARRecord::FARRecord(U1 cpu, U1 ver) {
    cpuType     = cpu;
    stdfVersion = ver;
}

U1 FARRecord::getCpuType(void) {
    return cpuType;
}

U1 FARRecord::getStdfVersion(void) {
    return stdfVersion;
}

RecordHeader FARRecord::getRecordHeader(void) {
    RecordHeader rec( FAR);
    rec.setLength(2);
    return rec;
}

ostream& operator<<(ostream& os, FARRecord& far) {
    return os << "\tCpu Type: " << (int) far.cpuType 
        << "\tSTDF Version: " << (int) far.stdfVersion;
}
stringstream& operator<<(stringstream& os, FARRecord& far) {
    os << "\tCpu Type: " << (int) far.cpuType << endl;
    os << "\tStdf Version: " << (int) far.stdfVersion << endl;
    return os;
}

STDFStream& operator>>(STDFStream& stdf, FARRecord& far) {
    stdf >> far.cpuType >> far.stdfVersion;
    return stdf;
}

STDFStream& operator<<(STDFStream& stdf, FARRecord& far) {
    RecordHeader rec = far.getRecordHeader();
    U1 nativeCpuType = 2;
    if(stdf.getByteOrder() == LittleEndian) {
        nativeCpuType = 2;
    }
    else if(stdf.getByteOrder() == BigEndian) {
        nativeCpuType = 1;
    }
    stdf << rec;
//    stdf << far.cpuType;
    stdf << nativeCpuType;
    stdf << far.stdfVersion;
    return stdf;
}


MIRRecord::MIRRecord(U2 len) {
    setupTime = startTime = 0;
    stationNumber = 0;
    burninTime = 0;
    modeCode = retestCode = protectionCode = ' ';

    recLength = len;
    time_t seconds;
    seconds = time(NULL);
    setupTime = startTime = seconds;
}

U2 MIRRecord::getLength(void) {
    U2 len = 0;
    len += 4 + 4 + 1 + 1 + 1 + 1 + 2 + 1;
    len += lotID.getLength();
    len += partType.getLength();
    len += nodeName.getLength();
    len += testerType.getLength();
    len += jobName.getLength();
    len += jobRevision.getLength();
    len += sublotID.getLength();
    len += operatorName.getLength();
    len += executiveType.getLength();
    len += executiveVersion.getLength();
    len += testCode.getLength();
    len += testTemp.getLength();
    len += userText.getLength();
    len += auxFile.getLength();
    len += packageType.getLength();
    len += familyID.getLength();
    len += dateCode.getLength();
    len += facilityID.getLength();
    len += floorID.getLength();
    len += processID.getLength();
    len += operationFreq.getLength();
    len += specName.getLength();
    len += specVersion.getLength();
    len += flowID.getLength();
    len += setupID.getLength();
    len += designRev.getLength();
    len += engineeringID.getLength();
    len += romCode.getLength();
    len += serialNumber.getLength();
    len += supervisorName.getLength();

    return len;
}

STDFStream& operator>>(STDFStream& stdf, MIRRecord& mir) {
    stdf.setRecCount( mir.recLength );

    stdf >> mir.setupTime >> mir.startTime; // 4 + 4 = 8
    stdf >> mir.stationNumber; // 1
    stdf >> mir.modeCode >> mir.retestCode >> mir.protectionCode; // 1 + 1 + 1 = 3
    stdf >> mir.burninTime; // 2
    stdf >> mir.commandCode; // 1

    stdf >> mir.lotID >> mir.partType >> mir.nodeName >> mir.testerType;
    stdf >> mir.jobName >> mir.jobRevision >> mir.sublotID >> mir.operatorName;

    stdf >> mir.executiveType >> mir.executiveVersion;

    stdf >> mir.testCode >> mir.testTemp;
    stdf >> mir.userText >> mir.auxFile >> mir.packageType >> mir.familyID;
    stdf >> mir.dateCode >> mir.facilityID >> mir.floorID >> mir.processID;
    stdf >> mir.operationFreq >> mir.specName >> mir.specVersion >> mir.flowID;
    stdf >> mir.setupID >> mir.designRev >> mir.engineeringID >> mir.romCode;
    stdf >> mir.serialNumber >> mir.supervisorName;

    stdf.setRecCount(0);
    return stdf;
}
STDFStream& operator<<(STDFStream& stdf, MIRRecord& mir) {
    RecordHeader rec(MIR);
    rec.setLength(mir.getLength());

    stdf << rec;
    stdf << mir.setupTime << mir.startTime;
    stdf << mir.stationNumber;
    stdf << mir.modeCode << mir.retestCode << mir.protectionCode;
    stdf << mir.burninTime;
    stdf << mir.commandCode;
    stdf << mir.lotID << mir.partType << mir.nodeName << mir.testerType;
    stdf << mir.jobName << mir.jobRevision << mir.sublotID << mir.operatorName;
    stdf << mir.executiveType << mir.executiveVersion << mir.testCode << mir.testTemp;
    stdf << mir.userText << mir.auxFile << mir.packageType << mir.familyID;
    stdf << mir.dateCode << mir.facilityID << mir.floorID << mir.processID;
    stdf << mir.operationFreq << mir.specName << mir.specVersion << mir.flowID;
    stdf << mir.setupID << mir.designRev << mir.engineeringID << mir.romCode;
    stdf << mir.serialNumber << mir.supervisorName;

    return stdf;
}

ostream& operator<<(ostream& os, MIRRecord& mir) {
    string sep = "\t";
    os << sep << "Setup Time: " << mir.setupTime << " " << time2string(mir.setupTime) << endl;
    os << sep << "Start Time: " << mir.startTime << " " << time2string(mir.startTime) << endl;
    os << sep << "Station Number: " << (int) mir.stationNumber << endl;
    os << sep << "Test Mode Code: " << "\'" << mir.modeCode << "\'" << endl;
    os << sep << "Lot Retest Code: " << "\'" << mir.retestCode << "\'" << endl;
    os << sep << "Data Protection Code: " << "\'" << mir.protectionCode << "\'" << endl;
    os << sep << "Burn-in Time (in minutes): " << mir.burninTime << endl;
    os << sep << "Command Code: " << "\'" << mir.commandCode << "\'" << endl;
    os << sep << "Lot ID: " << mir.lotID << endl;
    os << sep << "Part Type: " << mir.partType << endl;
    os << sep << "Node Name: " << mir.nodeName << endl;
    os << sep << "Tester Type: " << mir.testerType << endl;
    os << sep << "Job Name: " << mir.jobName << endl;
    os << sep << "Job Revision: " << mir.jobRevision << endl;
    os << sep << "SubLot ID: " << mir.sublotID << endl;
    os << sep << "Operator Name: " << mir.operatorName << endl;
    os << sep << "Tester Executive Software Type: " << mir.executiveType << endl;
    os << sep << "Tester exec software version number: " << mir.executiveVersion << endl;
    os << sep << "Test Code: " << mir.testCode << endl;
    os << sep << "Test Temperature: " << mir.testTemp << endl;
    os << sep << "Generic User Text: " << mir.userText << endl;
    os << sep << "Aux File: " << mir.auxFile << endl;
    os << sep << "Package Type: " << mir.packageType << endl;
    os << sep << "Family ID: " << mir.familyID << endl;
    os << sep << "Date Code: " << mir.dateCode << endl;
    os << sep << "Facility ID: " << mir.facilityID << endl;
    os << sep << "Floor ID: " << mir.floorID << endl;
    os << sep << "Process ID: " << mir.processID << endl;
    os << sep << "Operation Freq: " << mir.operationFreq << endl;
    os << sep << "Spec Name: " << mir.specName << endl;
    os << sep << "Spec Version: " << mir.specVersion << endl;
    os << sep << "Flow ID: " << mir.flowID << endl;
    os << sep << "Setup ID: " << mir.setupID << endl;
    os << sep << "Design Revision: " << mir.designRev << endl;
    os << sep << "Engineerinig ID: " << mir.engineeringID << endl;
    os << sep << "ROM Code: " << mir.romCode << endl;
    os << sep << "Serial Number: " << mir.serialNumber << endl;
    os << sep << "Supervisor Name: " << mir.supervisorName << endl;
    return os;
}

ATRRecord::ATRRecord(U2 len) {
    modTime = 0;
    commandLine.str = "";
    commandLine.count = 0;
    recLength = 0;
    if(len > 0) recLength = len;
}

U2 ATRRecord::getLength(void) {
    U2 len;
    len = 4 + commandLine.getLength();

    return len;
}

STDFStream& operator>>(STDFStream& stdf, ATRRecord& atr) {
    stdf.setRecCount(atr.recLength);

    stdf >> atr.modTime >> atr.commandLine;

    stdf.setRecCount(0);
    return stdf;
}
STDFStream& operator<<(STDFStream& stdf, ATRRecord& atr) {
    RecordHeader rec(ATR);
    rec.setLength(atr.getLength());
    stdf << rec;
    stdf << atr.modTime << atr.commandLine;
    return stdf;
}
ostream& operator<<(ostream& os, ATRRecord& atr) {
    string sep = "\t";
    os << sep << "Modification Time: " << atr.modTime << endl;
    os << sep << "Command Line: " << atr.commandLine << endl;
    return os;
}

//
//  SDR - Site Description Record
//
SDRRecord::SDRRecord(U2 len) {
    headNumber = siteGroup = siteCount = 0;
    recLength = 0;
    if(len > 0) recLength = len;
}

U2 SDRRecord::getLength(void) {
    U2 len;

    len = 1 + 1 + 1 + 1*siteCount;
    len += handlerType.getLength();
    len += handlerID.getLength();
    len += cardType.getLength();
    len += cardID.getLength();
    len += loadType.getLength();
    len += loadID.getLength();
    len += dibType.getLength();
    len += dibID.getLength();
    len += cableType.getLength();
    len += cableID.getLength();
    len += contactorType.getLength();
    len += contactorID.getLength();
    len += laserType.getLength();
    len += laserID.getLength();
    len += extraType.getLength();
    len += extraID.getLength();

    return len;
}

STDFStream& operator>>(STDFStream& stdf, SDRRecord& sdr) {
    stdf.setRecCount( sdr.recLength );

    stdf >> sdr.headNumber >> sdr.siteGroup >> sdr.siteCount;
    for(unsigned char i = 0; i<sdr.siteCount; i++) {
        U1 num;
        stdf >> num;
        sdr.siteNumbers.push_back(num);
    }
    stdf >> sdr.handlerType   >> sdr.handlerID;
    stdf >> sdr.cardType      >> sdr.cardID;
    stdf >> sdr.loadType      >> sdr.loadID;
    stdf >> sdr.dibType       >> sdr.dibID;
    stdf >> sdr.cableType     >> sdr.cableID;
    stdf >> sdr.contactorType >> sdr.contactorID;
    stdf >> sdr.laserType     >> sdr.laserID;
    stdf >> sdr.extraType     >> sdr.extraID;

    stdf.setRecCount(0);
    return stdf;
}
STDFStream& operator<<(STDFStream& stdf, SDRRecord& sdr) {
    RecordHeader rec(SDR);
    rec.setLength(sdr.getLength());
    stdf << rec;
    stdf << sdr.headNumber << sdr.siteGroup << sdr.siteCount;
    if(sdr.siteCount > 0) {
        std::vector<U1>::iterator iter;
        for(iter = sdr.siteNumbers.begin(); iter != sdr.siteNumbers.end(); iter++) {
            stdf << *iter;
        }
    }
    stdf << sdr.handlerType   << sdr.handlerID;
    stdf << sdr.cardType      << sdr.cardID;
    stdf << sdr.loadType      << sdr.loadID;
    stdf << sdr.dibType       << sdr.dibID;
    stdf << sdr.cableType     << sdr.cableID;
    stdf << sdr.contactorType << sdr.contactorID;
    stdf << sdr.laserType     << sdr.laserID;
    stdf << sdr.extraType     << sdr.extraID;

    return stdf;
}

ostream& operator<<(ostream& os, SDRRecord& sdr) {
    string sep = "\t";
    os << sep << "Test Head Number: " << (int) sdr.headNumber << endl;
    os << sep << "Site Group Number: " << (int) sdr.siteGroup << endl;
    os << sep << "Site Count: " << (int) sdr.siteCount << endl;

    os << sep << "Site Numbers (Array): [";
    std::vector<U1>::iterator iter;
    for(iter = sdr.siteNumbers.begin(); iter != sdr.siteNumbers.end(); iter++) {
        os << (int) *iter;
    }
    os << "]" << endl;

    os << sep << "Handler Type: " << sdr.handlerType << endl;
    os << sep << "Handler ID: " << sdr.handlerID << endl;
    os << sep << "Probe Card Type: " << sdr.cardType << endl;
    os << sep << "Probe Card ID: " << sdr.cardID << endl;
    os << sep << "Load Board Type: " << sdr.loadType << endl;
    os << sep << "Load Board ID: " << sdr.loadID << endl;
    os << sep << "DIB Board Type: " << sdr.dibType << endl;
    os << sep << "DIB Board ID: " << sdr.dibID << endl;
    os << sep << "Interface Cable Type: " << sdr.cableType << endl;
    os << sep << "Interface Cable ID: " << sdr.cableID << endl;
    os << sep << "Handler Contacter Type: " << sdr.contactorType << endl;
    os << sep << "Handler Contactor ID: " << sdr.contactorID << endl;
    os << sep << "Laser Type: " << sdr.laserType << endl;
    os << sep << "Laser ID: " << sdr.laserID << endl;
    os << sep << "Extra equipment Type: " << sdr.extraType << endl;
    os << sep << "Extra equipment ID: " << sdr.extraID << endl;
    return os;
}

PMRRecord::PMRRecord(U2 len) {
    index = 0;
    channelType = 0;
    channelName = "";
    headNum = siteNum = 1;

    recLength = 0;
    if(len > 0) recLength = len;
}

PMRRecord::PMRRecord(U2 ind, string cName, string pName, string lName, U1 hNum, U1 sNum, U2 cType) {
    index = ind;
    channelName = cName;
    physicalName = pName;
    logicalName = lName;
}

U2 PMRRecord::getLength(void) {
    U2 len = 0;
    len += 2 + 2;
    len += channelName.getLength();
    len += physicalName.getLength();
    len += logicalName.getLength();
    len += 1 + 1;

    return len;
}

RecordHeader PMRRecord::getRecordHeader(void) {
    RecordHeader rec( PMR );
    rec.setLength(getLength());
    return rec;
}

STDFStream& operator>>(STDFStream& stdf, PMRRecord& pmr) {
    stdf.setRecCount( pmr.recLength );

    stdf >> pmr.index >> pmr.channelType;
    stdf >> pmr.channelName;
    stdf >> pmr.physicalName;
    stdf >> pmr.logicalName;
    stdf >> pmr.headNum >> pmr.siteNum;

    stdf.setRecCount(0);
    return stdf;
}

ostream& operator<<(ostream& os, PMRRecord& pmr) {
    string sep = "\t";
    os << sep << "Index: " << (int) pmr.index << endl;
    os << sep << "Channel Type: " << pmr.channelType << endl;
    os << sep << "Channel Name: " << pmr.channelName << endl;
    os << sep << "Physical Name: " << pmr.physicalName << endl;
    os << sep << "Logical Name: " << pmr.logicalName << endl;
    os << sep << "Head Number: " << (int) pmr.headNum << endl;
    os << sep << "Site Number: " << (int) pmr.siteNum << endl;
    return os;
}

STDFStream& operator<<(STDFStream& stdf, PMRRecord& pmr) {
    RecordHeader rec = pmr.getRecordHeader();
    stdf << rec;
    stdf << pmr.index << pmr.channelType;
    stdf << pmr.channelName << pmr.physicalName << pmr.logicalName;
    stdf << pmr.headNum << pmr.siteNum;
    return stdf;
}

//
//  PGR - Pin Group Record
//

PGRRecord::PGRRecord(U2 len) {
    index      = 0;
    indexCount = 0;
    pmrIndexes.clear();
    recLength = 0;
    if(len > 0) recLength = len;
}
U2 PGRRecord::getLength(void) {
    U2 len = 2 + groupName.getLength() + 2;
    if(indexCount > 0) len += (indexCount * 2);

    return len;
}

RecordHeader PGRRecord::getRecordHeader(void) {
    RecordHeader rec(PGR);
    rec.setLength(getLength());

    return rec;
}

void PGRRecord::addPMRIndex(U2 ind) {
    // First check if it is a valid index TODO

    pmrIndexes.push_back(ind);
    indexCount++;
}

STDFStream& operator>>(STDFStream& stdf, PGRRecord& pgr) {
    stdf.setRecCount( pgr.recLength );

    stdf >> pgr.index;
    stdf >> pgr.groupName >> pgr.indexCount;
    for(U2 i = 0; i < pgr.indexCount; i++) {
        U2 num;
        stdf >> num;
        pgr.pmrIndexes.push_back(num);
    }

    stdf.setRecCount(0);
    return stdf;
}

ostream& operator<<(ostream& os, PGRRecord& pgr) {
    string sep = "\t";
    os << sep << "Index: " << (int) pgr.index << endl;
    os << sep << "Group Name: " << pgr.groupName << endl;

    os << sep << "Index Count: " << pgr.indexCount << endl;
    os << sep << "PMR Indexes: [ ";
    if(pgr.indexCount > 0) {
        std::vector<U2>::iterator iter;
        for(iter = pgr.pmrIndexes.begin(); iter != pgr.pmrIndexes.end(); iter++) {
            os << (int) *iter << " ";
        }
    }
    os << "]" << endl;

    return os;
}

STDFStream& operator<<(STDFStream& stdf, PGRRecord& pgr) {
    RecordHeader rec = pgr.getRecordHeader();
    stdf << rec;
    stdf << pgr.index << pgr.groupName;
    stdf << pgr.indexCount;
    if(pgr.indexCount > 0) {
        std::vector<U2>::iterator iter;
        for(iter = pgr.pmrIndexes.begin(); iter != pgr.pmrIndexes.end(); iter++) {
            stdf << *iter;
        }
    }
    return stdf;
}

PIRRecord::PIRRecord(U2 len) {
    headNumber = siteNumber = 0;
    recLength = 0;
    if(len > 0) recLength = len;
}

U2 PIRRecord::getLength(void) {
    U2 len = 2;
    return len;
}

STDFStream& operator>>(STDFStream& stdf, PIRRecord& pir) {
    stdf.setRecCount( pir.recLength );

    stdf >> pir.headNumber >> pir.siteNumber;

    stdf.setRecCount(0);
    return stdf;
}
STDFStream& operator<<(STDFStream& stdf, PIRRecord& pir) {
    RecordHeader rec(PIR);
    rec.setLength(pir.getLength());
    stdf << rec;
    stdf << pir.headNumber << pir.siteNumber;
    return stdf;
}
ostream& operator<<(ostream& os, PIRRecord& pir) {
    string sep = "\t";
    os << sep << "Head Number: " << (int) pir.headNumber << endl;
    os << sep << "Site Number: " << (int) pir.siteNumber << endl;
    return os;
}

FTRRecord::FTRRecord(U2 len) {
    testNumber = 0;
    headNumber = siteNumber = 0;
    testFlag = optFlag = 0;
    cycleCount = relVecAdd = repeatCount = numFail = 0;
    xFailAdd = yFailAdd = 0;
    vectorOffset = 0;
    rtnCount = pgmCount = 0;
    patGNumber = 255;

    recLength = 0;
    if(len > 0) recLength = len;
}

U2 FTRRecord::getLength(void) {
    U2 len = 0;
    len += 4 + 1 + 1 + 1 + 1 + 4 + 4 + 4 + 4 + 4 + 4 + 2 + 2 + 2;
    len += 2*rtnCount;
    len += (rtnCount/2) + (rtnCount % 2);
    len += 2*pgmCount;
    len += (pgmCount/2) + (pgmCount % 2);
    len += failPin.getLength();
    len += vectorName.getLength() + timeSet.getLength() + opCode.getLength() + testText.getLength();
    len += alarmID.getLength() + progText.getLength() + resultText.getLength();
    len += 1 + spinMap.getLength();

    return len;
}

STDFStream& operator>>(STDFStream& stdf, FTRRecord& ftr) {

    stdf.setRecCount(ftr.recLength);

    stdf >> ftr.testNumber >> ftr.headNumber >> ftr.siteNumber;
    stdf >> ftr.testFlag >> ftr.optFlag;
    stdf >> ftr.cycleCount >> ftr.relVecAdd >> ftr.repeatCount >> ftr.numFail;
    stdf >> ftr.xFailAdd >> ftr.yFailAdd;
    stdf >> ftr.vectorOffset;
    stdf >> ftr.rtnCount >> ftr.pgmCount;
    ftr.rtnIndexes.clear();
    ftr.rtnStates.clear();
    U2 val = 0; N1 state = 0;
    if(ftr.rtnCount > 0) {
        for(U2 j = 0; j<ftr.rtnCount; j++) {
            val = 0;
            stdf >> val;
            ftr.rtnIndexes.push_back(val);
        }
        for(U2 j = 1; j <= ((ftr.rtnCount+1)/2); j++) {
            state = 0;
            stdf >> state;
            ftr.rtnStates.push_back(state & 0x0F);
            if(2*j <= ftr.rtnCount)
                ftr.rtnStates.push_back(state & 0xF0);
        }
    }
    ftr.pgmIndexes.clear();
    ftr.pgmStates.clear();
    if(ftr.pgmCount > 0) {
        for(U2 k = 0; k<ftr.pgmCount; k++) {
            val = 0;
            stdf >> val;
            ftr.pgmIndexes.push_back(val);
        }
        for(U2 k = 1; k <= ((ftr.pgmCount+1)/2); k++) {
            state = 0;
            stdf >> state;
            ftr.pgmStates.push_back(state & 0x0F);
            if(2*k <= ftr.pgmCount)
                ftr.pgmStates.push_back(state & 0xF0);
        }
    }
    stdf >> ftr.failPin;
    stdf >> ftr.vectorName;
    stdf >> ftr.timeSet;
    stdf >> ftr.opCode;
    stdf >> ftr.testText;
    stdf >> ftr.alarmID;
    stdf >> ftr.progText;
    stdf >> ftr.resultText;
    stdf >> ftr.patGNumber;
    stdf >> ftr.spinMap;

    stdf.setRecCount(0);
    return stdf;
}
STDFStream& operator<<(STDFStream& stdf, FTRRecord& ftr) {
    RecordHeader rec(FTR);
    rec.setLength(ftr.getLength());
    stdf << rec;
    stdf << ftr.testNumber << ftr.headNumber << ftr.siteNumber;
    stdf << ftr.testFlag << ftr.optFlag;
    stdf << ftr.cycleCount << ftr.relVecAdd << ftr.repeatCount << ftr.numFail;
    stdf << ftr.xFailAdd << ftr.yFailAdd;
    stdf << ftr.vectorOffset;
    stdf << ftr.rtnCount << ftr.pgmCount;
    // TODO: Need to change this since we need to group nibbles
    for(U2 j = 0; j<ftr.rtnCount; j++) {
        stdf << ftr.rtnIndexes[j];
    }
    for(U2 j = 0; j<ftr.rtnCount; j++) {
        stdf << ftr.rtnStates[j];
    }
    for(U2 k = 0; k<ftr.pgmCount; k++) {
        stdf << ftr.pgmIndexes[k];
    }
    for(U2 k = 0; k<ftr.pgmCount; k++) {
        stdf << ftr.rtnStates[k];
    }
    stdf << ftr.failPin;
    stdf << ftr.vectorName << ftr.timeSet << ftr.opCode << ftr.testText;
    stdf << ftr.alarmID << ftr.progText << ftr.resultText;
    stdf << ftr.patGNumber << ftr.spinMap;
    return stdf;
}
ostream& operator<<(ostream& os, FTRRecord& ftr) {
    string sep = "\t";
    os << sep << "Test Number: " << (int) ftr.testNumber << endl;
    os << sep << "Head Number: " << (int) ftr.headNumber << endl;
    os << sep << "Site Number: " << (int) ftr.siteNumber << endl;
    os << sep << "Test Flag: " << toBinString( ftr.testFlag) << endl;
    os << sep << "Opt  Flag: " << toBinString( ftr.optFlag)  << endl;
    os << sep << "Cycle Count: " << (int) ftr.cycleCount << endl;
    os << sep << "Relative Vector Address: " << (int) ftr.relVecAdd << endl;
    os << sep << "Repeat Count: " << (int) ftr.repeatCount << endl;
    os << sep << "Num Fail: " << (int) ftr.numFail << endl;
    os << sep << "X Fail Address: " << (int) ftr.xFailAdd << endl;
    os << sep << "Y Fail Address: " << (int) ftr.yFailAdd << endl;
    os << sep << "Vector Offset: " << (int) ftr.vectorOffset << endl;
    os << sep << "Return Count: " << (int) ftr.rtnCount << endl;
    os << sep << "Program Count: " << (int) ftr.pgmCount << endl;
    os << sep << "Return Indexes:";
    std::vector<U2>::iterator iter1;
    for(iter1 = ftr.rtnIndexes.begin(); iter1 != ftr.rtnIndexes.end(); iter1++) {
        os << " " << (*iter1);
    }
    os << endl;
    os << sep << "Return States:";
    std::vector<N1>::iterator iter2;
    for(iter2 = ftr.rtnStates.begin(); iter2 != ftr.rtnStates.end(); iter2++) {
        os << " " << (int) (*iter2);
    }
    os << endl;

    os << sep << "Program Indexes:";
    for(iter1 = ftr.pgmIndexes.begin(); iter1 != ftr.pgmIndexes.end(); iter1++) {
        os << " " << (*iter1);
    }
    os << endl;
    os << sep << "Program States:";
    for(iter2 = ftr.pgmStates.begin(); iter2 != ftr.pgmStates.end(); iter2++) {
        os << " " << (int) (*iter2);
    }
    os << endl;

    os << sep << "Fail Pin: " << ftr.failPin << endl;
    os << sep << "Vector Name: " << ftr.vectorName << endl;
    os << sep << "Time Set: " << ftr.timeSet << endl;
    os << sep << "Op Code: " << ftr.opCode << endl;
    os << sep << "Test Text: " << ftr.testText << endl;
    os << sep << "Alarm ID: " << ftr.alarmID << endl;
    os << sep << "Prog Text: " << ftr.progText << endl;
    os << sep << "Result Text: " << ftr.resultText << endl;
    return os;
}

GDRRecord::GDRRecord(U2 len) {
    fieldCount = 0;
    genData.clear();

    recLength = len;
}

U2 GDRRecord::getLength(void) {
    U2 len = 0;
    len += 2; // Field Count
    std::vector<VN>::iterator iter;
    for(iter = genData.begin(); iter != genData.end(); iter++) {
        len += (*iter).getLength();
    }

    return len;
}

STDFStream& operator>>(STDFStream& stdf, GDRRecord& gdr) {
    stdf.setRecCount( gdr.recLength );

    gdr.genData.clear();
    stdf >> gdr.fieldCount;

    for(U2 i = 0; i < gdr.fieldCount; i++) {
        VN val;
        stdf >> val;
        gdr.genData.push_back(val);
    }

    stdf.setRecCount( 0 );
    return stdf;
}

STDFStream& operator<<(STDFStream& stdf, GDRRecord& gdr) {
    RecordHeader rec(GDR);
    rec.setLength(gdr.getLength());
    stdf << rec;

    stdf << gdr.fieldCount;
    std::vector<VN>::iterator iter;
    for(iter = gdr.genData.begin(); iter != gdr.genData.end(); iter++) {
        stdf << *iter;
    }

    return stdf;
}

ostream& operator<<(ostream& os, GDRRecord& gdr) {
    string sep = "\t";
    os << sep << "Field Count: " << gdr.fieldCount << endl;
    std::vector<VN>::iterator iter;
    for(iter = gdr.genData.begin(); iter != gdr.genData.end(); iter++) {
        os << sep << "Code: " << (int) (*iter).code << "\tData: " << (*iter) << endl;
    }

    return os;
}

PTRRecord::PTRRecord(U2 len) {
    testNumber = 0;
    headNumber = siteNumber = 0;
    testFlag = parmFlag = 0;
    result = 0.0;
    optFlag = 0xFF;
    resultScale = llmScale = hlmScale = 0;
    loLimit = hiLimit = 0;
    loSpec = hiSpec = 0.0;

    recLength = 0;
    if(len > 0) recLength = len;
}

U2 PTRRecord::getLength(void) {
    U2 len = 0;
    len += 4 + 1 + 1 + 1 + 1 + 4;
    len += testText.getLength();
    len += alarmID.getLength();
    len += 1;
    if(optFlag == 0xFF) {
    }
    else {
        len += 1 + 1 + 1 + 4 + 4;
        len += units.getLength();
        len += cResFmt.getLength();
        len += cLLMFmt.getLength();
        len += cHLMFmt.getLength();
        len += 4 + 4;
    }

    return len;
}

STDFStream& operator>>(STDFStream& stdf, PTRRecord& ptr) {

    stdf.setRecCount( ptr.recLength );

    stdf >> ptr.testNumber >> ptr.headNumber >> ptr.siteNumber;
    stdf >> ptr.testFlag >> ptr.parmFlag;
    stdf >> ptr.result;
    stdf >> ptr.testText >> ptr.alarmID;
    stdf >> ptr.optFlag;
    if(ptr.optFlag == 0xFF) {
    }
    else {
        stdf >> ptr.resultScale >> ptr.llmScale >> ptr.hlmScale;
        stdf >> ptr.loLimit >> ptr.hiLimit;
        stdf >> ptr.units >> ptr.cResFmt >> ptr.cLLMFmt >> ptr.cHLMFmt;
        stdf >> ptr.loSpec >> ptr.hiSpec;
    }

    stdf.setRecCount( 0 );
    return stdf;
}

STDFStream& operator<<(STDFStream& stdf, PTRRecord& ptr) {
    RecordHeader rec(PTR);
    rec.setLength(ptr.getLength());
    stdf << rec;

    stdf << ptr.testNumber << ptr.headNumber << ptr.siteNumber;
    stdf << ptr.testFlag << ptr.parmFlag;
    stdf << ptr.result;
    stdf << ptr.testText << ptr.alarmID;
    stdf << ptr.optFlag;
    if(ptr.optFlag == 0xFF) {
    }
    else {
        stdf << ptr.resultScale << ptr.llmScale << ptr.hlmScale;
        stdf << ptr.loLimit << ptr.hiLimit;
        stdf << ptr.units << ptr.cResFmt << ptr.cLLMFmt << ptr.cHLMFmt;
        stdf << ptr.loSpec << ptr.hiSpec;
    }

    return stdf;
}

ostream& operator<<(ostream& os, PTRRecord& ptr) {
    string sep = "\t";

    os << sep << "Test Number: " << (int) ptr.testNumber << endl;
    os << sep << "Head Number: " << (int) ptr.headNumber << endl;
    os << sep << "Site Number: " << (int) ptr.siteNumber << endl;
    os << sep << "Test Flag: " << (int) ptr.testFlag << endl;
    os << sep << "Parm Flag: " << (int) ptr.parmFlag << endl;
    os << sep << "Result: " << ptr.result << endl;
    os << sep << "Test Text: " << ptr.testText << endl;
    os << sep << "Alarm ID: " << ptr.alarmID << endl;
    os << sep << "Opt Flag: " << (int) ptr.optFlag << endl;
    os << sep << "Result Scale: " << (int) ptr.resultScale << endl;
    os << sep << "LLM Scale: " << (int) ptr.llmScale << endl;
    os << sep << "HLM Scale: " << (int) ptr.hlmScale << endl;
    os << sep << "Low Limit: " << ptr.loLimit << endl;
    os << sep << "High Limit: " << ptr.hiLimit << endl;
    os << sep << "Units: " << ptr.units << endl;
    os << sep << "C Result Format: " << ptr.cResFmt << endl;
    os << sep << "C LLM Format: " << ptr.cLLMFmt << endl;
    os << sep << "C HLM Format: " << ptr.cHLMFmt << endl;
    os << sep << "Low Spec: " << ptr.loSpec << endl;
    os << sep << "High Spec: " << ptr.hiSpec << endl;
    
    return os;
}

PRRRecord::PRRRecord(U2 len) {
//        U1 headNumber, siteNumber;
//        B1 partFlag;
//        U2 numberTest, hardBin, softBin;
//        I2 xCoord, yCoord;
//        U4 testTime;
//        CN partID, partText;
//        BN partFix;
    headNumber = siteNumber = 0;
    partFlag = 0;
    numTest = hardBin = softBin = 0;
    xCoord = yCoord = -32768;
    testTime = 0;
    recLength = 0;
    if(len > 0) recLength = len;
}

U2 PRRRecord::getLength(void) {
    U2 len = 0;

    len += 1 + 1 + 1 + 2 + 2 + 2;
    len += 2 + 2 + 4;
    len += partID.getLength();
    len += partText.getLength();
    len += partFix.getLength();

    return len;
}

STDFStream& operator>>(STDFStream& stdf, PRRRecord& prr) {
    stdf.setRecCount( prr.recLength );

    stdf >> prr.headNumber >> prr.siteNumber;
    stdf >> prr.partFlag;
    stdf >> prr.numTest;
    stdf >> prr.hardBin >> prr.softBin;
    stdf >> prr.xCoord >> prr.yCoord;
    stdf >> prr.testTime;
    stdf >> prr.partID >> prr.partText >> prr.partFix;

    stdf.setRecCount(0);
    return stdf;
}
STDFStream& operator<<(STDFStream& stdf, PRRRecord& prr) {
    RecordHeader rec(PRR);
    rec.setLength(prr.getLength());
    stdf << rec;

    stdf << prr.headNumber << prr.siteNumber;
    stdf << prr.partFlag;
    stdf << prr.numTest;
    stdf << prr.hardBin << prr.softBin;
    stdf << prr.xCoord << prr.yCoord;
    stdf << prr.testTime;
    stdf << prr.partID << prr.partText << prr.partFix;

    return stdf;
}

ostream& operator<<(ostream& os, PRRRecord& prr) {
    string sep = "\t";

    os << sep << "Head Number: " << (int) prr.headNumber << endl;
    os << sep << "Site Number: " << (int) prr.siteNumber << endl;
    os << sep << "Part Flag: " << (int) prr.partFlag << endl;
    os << sep << "Num Test: " << (int) prr.numTest << endl;
    os << sep << "Hard Bin: " << (int) prr.hardBin << endl;
    os << sep << "Soft Bin: " << (int) prr.softBin << endl;
    os << sep << "X Coord: " << (int) prr.xCoord << endl;
    os << sep << "Y Coord: " << (int) prr.yCoord << endl;
    os << sep << "Test Time: " << (int) prr.testTime << endl;
    os << sep << "Part ID: " << prr.partID << endl;
    os << sep << "Part Text: " << prr.partText << endl;
    os << sep << "Part Fix: " << prr.partFix<< endl;


    return os;
}

MPRRecord::MPRRecord(U2 len) {
    testNumber = 0;
    headNumber = siteNumber = 0;
    testFlag = parmFlag = 0;
    indexCount = resultCount = 0;
    optFlag = 0;
    resultScale = llmScale = hlmScale = 0;
    loLimit = hiLimit = 0;
    startIn = incrIn = 0;
    loSpec = hiSpec = 0;

    recLength = 0;
    if(len > 0) recLength = len;
}

U2 MPRRecord::getLength(void) {
    U2 len = 0;
    len += 4 + 1 + 1 + 1 + 1 + 2 + 2;
    len += (indexCount/2) + (indexCount % 2);
    len += resultCount*4;
    len += testText.getLength();
    len += alarmID.getLength();
    len += 1; // optFlag
    if(optFlag == 0xFF) {
    }
    else {
        len += 1 + 1 + 1 + 4 + 4 + 4 + 4;
        len += indexCount*2;
        len += units.getLength();
        len += unitsIn.getLength();
        len += cResFmt.getLength();
        len += cLLMFmt.getLength();
        len += cHLMFmt.getLength();
        len += 4 + 4;
    }

    return len;
}

STDFStream& operator>>(STDFStream& stdf, MPRRecord& mpr) {
    stdf.setRecCount( mpr.recLength );

    stdf >> mpr.testNumber >> mpr.headNumber >> mpr.siteNumber;
    stdf >> mpr.testFlag >> mpr.parmFlag;
    stdf >> mpr.indexCount >> mpr.resultCount;
    mpr.rtnStates.clear();
    mpr.rtnResults.clear();
    N1 state = 0;
    if(mpr.indexCount > 0) {
        for(U2 j = 1; j <= ((mpr.indexCount+1)/2); j++) {
            state = 0;
            stdf >> state;
            mpr.rtnStates.push_back(state & 0x0F);
            if(2*j <= mpr.indexCount)
                mpr.rtnStates.push_back(state & 0xF0);
        }
    }
    for(U2 k = 0; k < mpr.resultCount; k++) {
        R4 val = 0.0;
        stdf >> val;
        mpr.rtnResults.push_back(val);
    }
    stdf >> mpr.testText >> mpr.alarmID;
    stdf >> mpr.optFlag;
    if(mpr.optFlag != 0xFF) {
        stdf >> mpr.resultScale >> mpr.llmScale >> mpr.hlmScale;
        stdf >> mpr.loLimit >> mpr.hiLimit;
        stdf >> mpr.startIn >> mpr.incrIn;
        U2 val = 0;
        for(U2 j = 0; j < mpr.indexCount; j++) {
            stdf >> val;
            mpr.pmrIndexes.push_back(val);
        }
        stdf >> mpr.units >> mpr.unitsIn;
        stdf >> mpr.cResFmt >> mpr.cLLMFmt >> mpr.cHLMFmt;
        stdf >> mpr.loSpec >> mpr.hiSpec;
    }
    
    stdf.setRecCount( 0 );
    return stdf;
}
STDFStream& operator<<(STDFStream& stdf, MPRRecord& mpr) {
    RecordHeader rec(MPR);
    rec.setLength(mpr.getLength());
    stdf << rec;

    return stdf;
}

ostream& operator<<(ostream& os, MPRRecord& mpr) {
    string sep = "\t";

    os << sep << "Test Number: " << (int) mpr.testNumber << endl;
    os << sep << "Head Number: " << (int) mpr.headNumber << endl;
    os << sep << "Site Number: " << (int) mpr.siteNumber << endl;
    os << sep << "Test Flag: " << toBinString( mpr.testFlag) << endl;
    os << sep << "Parm Flag: " << toBinString( mpr.parmFlag) << endl;

    os << sep << "Opt  Flag: " << toBinString( mpr.optFlag)  << endl;
    return os;
}

TSRRecord::TSRRecord(U2 len) {
    headNumber = siteNumber = 0;
    testNumber = execCount = failCount = alarmCount = 0;
    optFlag = 0xFF;
    testTime = testMin = testMax = testSum = testSquare = 0.0;

    recLength = 0;
    if(len > 0) recLength = len;
}

U2 TSRRecord::getLength(void) {
    U2 len = 0;
    len += 1 + 1 + 1 + 4*4;
    len += testName.getLength();
    len += seqName.getLength();
    len += testLabel.getLength();
    len += 1 + 5*4;

    return len;
}

STDFStream& operator>>(STDFStream& stdf, TSRRecord& tsr) {
    stdf.setRecCount( tsr.recLength );

    stdf >> tsr.headNumber >> tsr.siteNumber;
    stdf >> tsr.testType;
    stdf >> tsr.testNumber >> tsr.execCount >> tsr.failCount >> tsr.alarmCount;
    stdf >> tsr.testName >> tsr.seqName >> tsr.testLabel;
    stdf >> tsr.optFlag;
    stdf >> tsr.testTime >> tsr.testMin >> tsr.testMax >> tsr.testSum >> tsr.testSquare;

    stdf.setRecCount( 0);
    return stdf;
}

STDFStream& operator<<(STDFStream& stdf, TSRRecord& tsr) {
    RecordHeader rec(TSR);
    rec.setLength(tsr.getLength());
    stdf << rec;

    stdf << tsr.headNumber << tsr.siteNumber;
    stdf << tsr.testType;
    stdf << tsr.testNumber << tsr.execCount << tsr.failCount << tsr.alarmCount;
    stdf << tsr.testName << tsr.seqName << tsr.testLabel;
    stdf << tsr.optFlag;
    stdf << tsr.testTime << tsr.testMin << tsr.testMax 
        << tsr.testSum << tsr.testSquare;

    return stdf;
}

ostream& operator<<(ostream& os, TSRRecord& tsr) {
    string sep = "\t";

    os << sep << "Head Number: " << (int) tsr.headNumber << endl;
    os << sep << "Site Number: " << (int) tsr.siteNumber << endl;
    os << sep << "Test Type: " << tsr.testType << endl;
    os << sep << "Test Number: " << (int) tsr.testNumber << endl;
    os << sep << "Exec Count: " << (int) tsr.execCount << endl;
    os << sep << "Fail Count: " << (int) tsr.failCount << endl;
    os << sep << "Alarm Count: " << (int) tsr.alarmCount << endl;
    os << sep << "Test Name: " << tsr.testName << endl;
    os << sep << "Sequencer Name: " << tsr.seqName << endl;
    os << sep << "Test Label: " << tsr.testLabel << endl;
    os << sep << "Opt  Flag: " << toBinString( tsr.optFlag)  << endl;
    os << sep << "Test Time: " << tsr.testTime << endl;
    os << sep << "Test Min: " << tsr.testMin << endl;
    os << sep << "Test Max: " << tsr.testMax << endl;
    os << sep << "Test Sum: " << tsr.testSum << endl;
    os << sep << "Test Squares: " << tsr.testSquare << endl;

    return os;
}

DTRRecord::DTRRecord(U2 len) {
    recLength = len;
}

U2 DTRRecord::getLength(void) {
    U2 len = 0;
    len += text.getLength();

    return len;
}

STDFStream& operator>>(STDFStream& stdf, DTRRecord& dtr) {
    stdf.setRecCount( dtr.recLength );
    stdf >> dtr.text;

    stdf.setRecCount(0);
    return stdf;
}

STDFStream& operator<<(STDFStream& stdf, DTRRecord& dtr) {
    RecordHeader rec(DTR);
    rec.setLength(dtr.getLength());
    stdf << rec;

    stdf << dtr.text;
    return stdf;
}
ostream& operator<<(ostream& os, DTRRecord& dtr) {
    string sep = "\t";

    os << sep << "Text: " << dtr.text << endl;

    return os;
}

SBRRecord::SBRRecord(U2 len) {
    headNumber = siteNumber = 0;
    sbinNumber = 0;
    sbinCount = 0;
    sbinPF = ' ';
    recLength = len;
}

U2 SBRRecord::getLength(void) {
    U2 len = 0;
    len += 1 + 1 + 2 + 4 + 1 + sbinName.getLength();

    return len;
}

STDFStream& operator>>(STDFStream& stdf, SBRRecord& sbr) {
    stdf.setRecCount( sbr.recLength );

    stdf >> sbr.headNumber >> sbr.siteNumber;
    stdf >> sbr.sbinNumber;
    stdf >> sbr.sbinCount;
    stdf >> sbr.sbinPF >> sbr.sbinName;

    stdf.setRecCount(0);
    return stdf;
}

STDFStream& operator<<(STDFStream& stdf, SBRRecord& sbr) {
    RecordHeader rec(SBR);
    rec.setLength(sbr.getLength());
    stdf << rec;

    stdf << sbr.headNumber << sbr.siteNumber;
    stdf << sbr.sbinNumber;
    stdf << sbr.sbinCount;
    stdf << sbr.sbinPF << sbr.sbinName;
    return stdf;
}
ostream& operator<<(ostream& os, SBRRecord& sbr) {
    string sep = "\t";

    os << sep << "Head Number: " << (int) sbr.headNumber << endl;
    os << sep << "Site Number: " << (int) sbr.siteNumber << endl;
    os << sep << "SBin Number: " << (int) sbr.sbinNumber << endl;
    os << sep << "SBin Count: "  << (int) sbr.sbinCount  << endl;
    os << sep << "SBin P/F: "    << sbr.sbinPF  << endl;
    os << sep << "SBin Name: "   << sbr.sbinName  << endl;

    return os;
}


HBRRecord::HBRRecord(U2 len) {
    headNumber = siteNumber = 0;
    hbinNumber = 0;
    hbinCount = 0;
    hbinPF = ' ';
    recLength = len;
}

U2 HBRRecord::getLength(void) {
    U2 len = 0;
    len += 1 + 1 + 2 + 4 + 1 + hbinName.getLength();

    return len;
}

STDFStream& operator>>(STDFStream& stdf, HBRRecord& hbr) {
    stdf.setRecCount( hbr.recLength );

    stdf >> hbr.headNumber >> hbr.siteNumber;
    stdf >> hbr.hbinNumber;
    stdf >> hbr.hbinCount;
    stdf >> hbr.hbinPF >> hbr.hbinName;

    stdf.setRecCount(0);
    return stdf;
}

STDFStream& operator<<(STDFStream& stdf, HBRRecord& hbr) {
    RecordHeader rec(HBR);
    rec.setLength(hbr.getLength());
    stdf << rec;

    stdf << hbr.headNumber << hbr.siteNumber;
    stdf << hbr.hbinNumber;
    stdf << hbr.hbinCount;
    stdf << hbr.hbinPF << hbr.hbinName;
    return stdf;
}
ostream& operator<<(ostream& os, HBRRecord& hbr) {
    string sep = "\t";

    os << sep << "Head Number: " << (int) hbr.headNumber << endl;
    os << sep << "Site Number: " << (int) hbr.siteNumber << endl;
    os << sep << "HBin Number: " << (int) hbr.hbinNumber << endl;
    os << sep << "HBin Count: "  << (int) hbr.hbinCount  << endl;
    os << sep << "HBin P/F: "    << hbr.hbinPF  << endl;
    os << sep << "HBin Name: "   << hbr.hbinName  << endl;

    return os;
}

BPSRecord::BPSRecord(U2 len) {
    recLength = len;
}

U2 BPSRecord::getLength(void) {
    return seqName.getLength();
}

STDFStream& operator>>(STDFStream& stdf, BPSRecord& bps) {
    stdf.setRecCount( bps.recLength );

    stdf >> bps.seqName;

    stdf.setRecCount(0);
    return stdf;
}

STDFStream& operator<<(STDFStream& stdf, BPSRecord& bps) {
    RecordHeader rec(BPS);
    rec.setLength(bps.getLength());
    stdf << rec;

    stdf << bps.seqName;
    return stdf;
}
ostream& operator<<(ostream& os, BPSRecord& bps) {
    string sep = "\t";

    os << sep << "Sequencer Name: " << bps.seqName << endl;
    return os;
}


EPSRecord::EPSRecord(U2 len) {
    recLength = 0;
}

U2 EPSRecord::getLength(void) {
    return 0;
}

STDFStream& operator>>(STDFStream& stdf, EPSRecord& eps) {
    return stdf;
}

STDFStream& operator<<(STDFStream& stdf, EPSRecord& eps) {
    RecordHeader rec(EPS);
    rec.setLength(eps.getLength());
    stdf << rec;

    return stdf;
}
ostream& operator<<(ostream& os, EPSRecord& eps) {
    string sep = "\t";

//    os << sep << "Sequencer Name: " << eps.seqName << endl;
    return os;
}

MRRRecord::MRRRecord(U2 len) {
//    U4 finishTime;
//    C1 dispCode;
//    CN userDesc, execDesc;
    finishTime = 0;
    dispCode = ' ';
    recLength = len;
}

U2 MRRRecord::getLength(void) {
    U2 len = 0;
    len += 4 + 1 + userDesc.getLength() + execDesc.getLength();
    return len;
}

STDFStream& operator>>(STDFStream& stdf, MRRRecord& mrr) {
    stdf.setRecCount( mrr.recLength );

    stdf >> mrr.finishTime;
    stdf >> mrr.dispCode;
    stdf >> mrr.userDesc >> mrr.execDesc;

    stdf.setRecCount(0);
    return stdf;
}

STDFStream& operator<<(STDFStream& stdf, MRRRecord& mrr) {
    RecordHeader rec(MRR);
    rec.setLength(mrr.getLength());
    stdf << rec;

    stdf << mrr.finishTime;
    stdf << mrr.dispCode;
    stdf << mrr.userDesc << mrr.execDesc;

    return stdf;
}
ostream& operator<<(ostream& os, MRRRecord& mrr) {
    string sep = "\t";

    os << sep << "Finish Time: " << (int) mrr.finishTime << " " << time2string(mrr.finishTime) << endl;
    os << sep << "Disposition Code: " << mrr.dispCode << endl;
    os << sep << "User Description: " << mrr.userDesc << endl;
    os << sep << "Executive Description: " << mrr.execDesc << endl;
    return os;
}

PCRRecord::PCRRecord(U2 len) {
//    U1 headNumber, siteNumber;
//    U4 partCount, retestCount, abortCount, goodCount, funcCount;
    headNumber = siteNumber = 0;
    partCount = retestCount = abortCount = goodCount = funcCount = 0;
    recLength = len;
}

U2 PCRRecord::getLength(void) {
    U2 len = 0;
    len += 2*1 + 5*4;
    return len;
}

STDFStream& operator>>(STDFStream& stdf, PCRRecord& pcr) {
    stdf.setRecCount( pcr.recLength );

    stdf >> pcr.headNumber >> pcr.siteNumber;
    stdf >> pcr.partCount >> pcr.retestCount >> pcr.abortCount >> pcr.goodCount >> pcr.funcCount;

    stdf.setRecCount(0);
    return stdf;
}

STDFStream& operator<<(STDFStream& stdf, PCRRecord& pcr) {
    RecordHeader rec(PCR);
    rec.setLength(pcr.getLength());
    stdf << rec;

    stdf << pcr.headNumber << pcr.siteNumber;
    stdf << pcr.partCount << pcr.retestCount << pcr.abortCount << pcr.goodCount << pcr.funcCount;

    return stdf;
}
ostream& operator<<(ostream& os, PCRRecord& pcr) {
    string sep = "\t";

    os << sep << "Head Number: " << (int) pcr.headNumber << endl;
    os << sep << "Site Number: " << (int) pcr.siteNumber << endl;
    os << sep << "Part Count: " << (int) pcr.partCount << endl;
    os << sep << "Retest Count: " << (int) pcr.retestCount << endl;
    os << sep << "Abort Count: " << (int) pcr.abortCount << endl;
    os << sep << "Good Count: " << (int) pcr.goodCount << endl;
    os << sep << "Func Count: " << (int) pcr.funcCount << endl;

    return os;
}

PLRRecord::PLRRecord(U2 len) {
}

U2 PLRRecord::getLength(void) {
    U2 len = 0;
    len += 2;
    return len;
}


