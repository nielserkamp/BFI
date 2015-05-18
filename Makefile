GCC=g++
CFLAGS=-fPIE -I./include -I./ -g -Wall -std=c++0x
GLL=g++
LFLAGS=-L./libs -ltracer -ludis86

OBJECTS=AddressRangeList.o\
CFG.o\
Instruction.o\
Location/MemoryLocation.o\
Location/RegisterLocation.o\
Location/InstructionLocation.o\
Analysis/Live.o\
Analysis/Analysis.o\
Analysis/BackwardReadAnalysis.o\
Analysis/ForwardWriteAnalysis.o\
LivenessArray.o\
TestCase/TestCase.o\
TestCase/strcmpTestCase.o\
TestCase/memcmpTestCase.o\
TestCase/memcpyTestCase.o\
TestCase/inet_ntoaTestCase.o\
TestCase/mktimeTestCase.o\
TestCase/strtolTestCase.o\
TestCase/qsortTestCase.o\
TestCase/fgetsTestCase.o\
TestCase/sprintfTestCase.o\
TestCase/strftimeTestCase.o

all: bin/MasterTrimmed bin/Master bin/Slave $(OBJECTS)

bin/Master: Master.o $(OBJECTS)
	$(GLL) -o $@ $^ $(LFLAGS)

bin/MasterTrimmed: MasterTrimmed.o $(OBJECTS)
	$(GLL) -o $@ $^ $(LFLAGS)

bin/Slave: Slave.o $(OBJECTS)
	$(GLL) --pie -o $@ $^ $(LFLAGS)

%.o: %.cpp
	$(GCC) $(CFLAGS) -c -o $@ $< 

clean:
	rm *.o bin/Master bin/Slave $(OBJECTS)
