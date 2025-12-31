//
//  Author: Tushar Jog
//  Timestamp: <STDFFile.cpp Thu 2005/07/28 22:24:45 tjog>
//

#include "STDFFile.h"

//STDFFile::STDFFile(void) {
STDFFile::STDFFile(string file, RWMode md) {
    if(file != "") {
        fileName = file;
        mode = md;
    }
}

void STDFFile::setFile(string name, RWMode md) {
    if(name != "") {
        fileName = name;
        mode = md;
    }
}

bool STDFFile::getRecord(void) {
    return true;
}


