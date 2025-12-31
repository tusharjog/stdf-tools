#!/usr/bin/csh -f
gvim defines.h globals.h globals.cpp STDF.h STDF.cpp &
gvim STDFStream.h STDFStream.cpp main.cpp &
kpdf ~/docs/stdf/stdf*.pdf &

