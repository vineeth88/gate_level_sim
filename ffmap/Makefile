BENCH = b07
DEFINE_BENCH = -D__${BENCH}
BENCH_DIR = ./${BENCH}
LIBS =  ${BENCH_DIR}/Vtop__ALL.a

CXX = g++
CFLAGS = -c -Wall $(DEFINE_BENCH)

SYSTEMPERL = /home/vineeth/Tools/SystemPerl/src
VERILATOR_INCLUDE = /usr/local/share/verilator/include

INCLUDE = -I$(SYSTEMPERL) -I$(VERILATOR_INCLUDE)

DEBUG = -g -O0
CFLAGS += ${DEBUG}

all: gen

OBJS_GEN = circuit.o \
${BENCH}Int.o rtLevelSim.o \
gateLevelSim.o \
mapFF.o \
verilated.o SpCoverage.o
TARGET_GEN = mapFF${BENCH}

gen: $(OBJS_GEN)
	$(CXX) $(OBJS_GEN) $(INCLUDE) $(LIBS) -o $(TARGET_GEN) -pg -lm -lstdc++ 2>&1 | c++filt

# All object files below

circuit.o: incl.h circuit.h circuit.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o circuit.o circuit.cpp

${BENCH}Int.o: incl.h circuit.h ${BENCH}Int.h ${BENCH}Int.cpp circuit.h incl.h
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o ${BENCH}Int.o ${BENCH}Int.cpp

rtLevelSim.o: incl.h circuit.h ${BENCH}Int.h ${BENCH}Int.cpp rtLevelSim.h rtLevelSim.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o rtLevelSim.o rtLevelSim.cpp

gateLevelSim.o: incl.h circuit.h gateLevelSim.h gateLevelSim.cpp 
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o gateLevelSim.o gateLevelSim.cpp

mapFF.o: incl.h circuit.h gateLevelSim.h rtLevelSim.h ffMap.cpp
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0 -DVL_DEBUG=1 -c -o mapFF.o ffMap.cpp

verilated.o:
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0       -DVL_DEBUG=1  -c -o verilated.o ${VERILATOR_INCLUDE}/verilated.cpp

SpCoverage.o:
	$(CXX) $(CFLAGS) -I. -MMD $(INCLUDE) -DVL_PRINTF=printf -DVM_TRACE=0 -DVM_COVERAGE=0       -DVL_DEBUG=1  -c -o SpCoverage.o ${SYSTEMPERL}/SpCoverage.cpp

clean:
	rm -f *.o
	rm -f *.d
