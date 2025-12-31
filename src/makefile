
CPP = g++
OBJS = main.o STDFStream.o STDF.o globals.o application.o STDFFile.o
WARNINGS = -Wall
QTDIR = /usr/local/Trolltech/Qt-4.0.0
INCLUDES = -I$(QTDIR)/include
LIBS = -L$(QTDIR)/lib
OPT = 

all: stdf_reader

stdf_viewer: application.o STDFFile.o STDFStream.o STDF.o globals.o
	$(CPP) $(OPT) $^ $(LIBS) -o $@

stdf_reader: main.o STDFFile.o STDFStream.o STDF.o globals.o
	$(CPP) $^ $(OPT) -o $@

main.o: main.cpp
	$(CPP) $(WARNINGS) $(OPT) -c $< -o $@

application.o: application.cpp
	$(CPP) $(WARNINGS) $(OPT) $(INCLUDES) -c $< -o $@

STDFFile.o: STDFFile.cpp
	$(CPP) $(WARNINGS) $(OPT) -c $< -o $@
	
STDFStream.o: STDFStream.cpp
	$(CPP) $(WARNINGS) $(OPT) -c $< -o $@
	
STDF.o: STDF.cpp
	$(CPP) $(WARNINGS) $(OPT) -c $< -o $@
	
globals.o: globals.cpp
	$(CPP) $(WARNINGS) $(OPT) -c $< -o $@
	

clean:
	rm -f $(OBJS)
	rm -f stdf_reader stdf_viewer

