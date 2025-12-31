//
//  Author: Tushar Jog
//  Timestamp: <STDFFile.h Thu 2005/07/28 22:24:17 tjog>
//

#ifndef _STDFFILE_H_
#define _STDFFILE_H_

#include "defines.h"
#include "globals.h"
#include "STDF.h"
#include "STDFStream.h"

//
//  This class will the basic programming interface to a STDF file
//

class STDFFile {
    public:
        enum RWMode { None, ReadOnly, WriteOnly };
        enum Status { OK, ReadFail, WriteFail };
        STDFFile(string file = "", RWMode md = STDFFile::None);

        void setFile(string name, RWMode md = STDFFile::None);
    protected:
        bool getRecord(void);
    private:
        std::string fileName;
        STDFStream sstream;

        RWMode mode;
        Status status;
};


#endif

