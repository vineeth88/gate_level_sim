BENCH = b12
DEFINE_BENCH = -D__${BENCH}
BENCH_DIR = ./lib/${BENCH}
LIBS =  ${BENCH_DIR}/cpp/Vtop__ALL.a

CXX = g++
CFLAGS = -c -Wall $(DEFINE_BENCH)

SYSTEMPERL = /home/vineeth/Tools/SystemPerl/src
VERILATOR_INCLUDE = /usr/local/share/verilator/include

INCL_DIR = ./include
SRC_DIR = ./src
EXEC_DIR = ./exec
INCLUDE = -I$(SYSTEMPERL) -I$(VERILATOR_INCLUDE) -I$(INCL_DIR)

DEBUG = -g -pg -O0
CFLAGS += ${DEBUG}

all: gen

OBJS_GEN = circuit.o \
${BENCH}Int.o rtLevelSim.o \
verilated.o SpCoverage.o \
gateLevelSim.o \
genDist.o
#mapFF.o
TARGET_GEN = ${BENCH}DSGen
#TARGET_GEN = ${BENCH}RTLSim

OBJS_TRACE = circuit.o \
${BENCH}Int.o rtLevelSim.o \
gateLevelSim.o \
trace.o \
verilated.o SpCoverage.o
TARGET_TRACE = ${BENCH}trace

gen: $(OBJS_GEN)
	$(CXX) $(OBJS_GEN) $(INCLUDE) $(LIBS) -o $(EXEC_DIR)/$(TARGET_GEN) -pg -lm -lstdc++ 2>&1 | c++filt

trace: ${OBJS_TRACE}
	$(CXX) $(OBJS_TRACE) $(INCLUDE) $(LIBS) -o $(TARGET_TRACE) -pg -lm -lstdc++ 2>&1 | c++filt
	
# All object files below

circuit.o: $(INCL_DIR)/incl.h $(INCL_DIR)/circuit.h $(SRC_DIR)/circuit.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o circuit.o $(SRC_DIR)/circuit.cpp

${BENCH}Int.o: $(INCL_DIR)/incl.h $(INCL_DIR)/circuit.h $(INCL_DIR)/${BENCH}Int.h $(SRC_DIR)/${BENCH}Int.cpp $(INCL_DIR)/circuit.h $(INCL_DIR)/incl.h
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o ${BENCH}Int.o $(SRC_DIR)/${BENCH}Int.cpp

rtLevelSim.o: $(INCL_DIR)/incl.h $(INCL_DIR)/circuit.h $(INCL_DIR)/${BENCH}Int.h $(SRC_DIR)/${BENCH}Int.cpp $(INCL_DIR)/rtLevelSim.h $(SRC_DIR)/rtLevelSim.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o rtLevelSim.o $(SRC_DIR)/rtLevelSim.cpp

gateLevelSim.o: $(INCL_DIR)/incl.h $(INCL_DIR)/circuit.h $(INCL_DIR)/gateLevelSim.h $(SRC_DIR)/gateLevelSim.cpp 
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o gateLevelSim.o $(SRC_DIR)/gateLevelSim.cpp

#mapFF.o: $(INCL_DIR)/incl.h $(INCL_DIR)/circuit.h $(INCL_DIR)/gateLevelSim.h $(INCL_DIR)/rtLevelSim.h $(SRC_DIR)/ffMap.cpp
#	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o mapFF.o $(SRC_DIR)/ffMap.cpp

genDist.o: $(INCL_DIR)/incl.h $(INCL_DIR)/circuit.h $(INCL_DIR)/gateLevelSim.h $(INCL_DIR)/rtLevelSim.h $(SRC_DIR)/genDistSeq.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o genDist.o $(SRC_DIR)/genDistSeq.cpp

verilated.o:
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0       -DVL_DEBUG=1  -c -o verilated.o ${VERILATOR_INCLUDE}/verilated.cpp

SpCoverage.o:
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0       -DVL_DEBUG=1  -c -o SpCoverage.o ${SYSTEMPERL}/SpCoverage.cpp

procSig:
	$(CXX) -g -I./include -o $(EXEC_DIR)/sigFileProc $(SRC_DIR)/procSigFile.cpp

fsimNew:
	$(CXX) -g -o $(EXEC_DIR)/fsimNew $(SRC_DIR)/faultsim.cpp

clean:
	rm -f *.o
	rm -f *.d
